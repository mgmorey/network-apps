// Copyright (C) 2022  "Michael G. Morey" <mgmorey@gmail.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "network/assert.h"             // assert()
#include "network/network.h"            // Address, Context, Error,
                                        // HostVector, Hostname,
                                        // OsErrorResult, Overloaded,
                                        // SocketFamily, SocketHints,
                                        // get_hosts(),
                                        // get_sa_size_maximum(),
                                        // get_sa_size_minimum(),
                                        // sun_len_max, sun_len_min,
                                        // to_bytestring(), validate()
#include "network/os-features.h"        // HAVE_SOCKADDR_SA_LEN
#include "network/parse.h"              // parse()

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                            // IPPROTO_TCP, SOCK_STREAM, sockaddr,
                            // sockaddr_in, sockaddr_in6,
                            // sockaddr_storage
#include <ws2tcpip.h>       // AI_ADDRCONFIG, AI_CANONNAME
#else
#include <netdb.h>          // AI_ADDRCONFIG, AI_CANONNAME
#include <netinet/in.h>     // IPPROTO_TCP
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                            // SOCK_STREAM, sockaddr, sockaddr_in,
                            // sockaddr_in6, sockaddr_storage,
                            // sockaddr_un
#endif

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <cstring>      // std::memset()
#include <exception>    // std::exception
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <string>       // std::string
#include <variant>      // std::visit()

namespace TestAddress
{
    using Network::Address;
    using Network::Context;
    using Network::Error;
    using Network::HostVector;
    using Network::Hostname;
    using Network::OsErrorResult;
    using Network::Overloaded;
    using Network::SocketFamily;
    using Network::SocketHints;
    using Network::get_hosts;
    using Network::get_sa_size_maximum;
    using Network::get_sa_size_minimum;
    using Network::parse;
    using Network::sa_size;
    using Network::sin6_size;
    using Network::sin_size;
#ifndef WIN32
    using Network::sun_len_max;
    using Network::sun_len_min;
    using Network::sun_path_offset;
    using Network::sun_size;
#endif
    using Network::to_bytestring;
    using Network::validate;

    static constexpr auto expected_error_sa_family {
        "Invalid IP domain socket address"
    };
    static constexpr auto expected_error_sa_length_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of sa_len_type)"
    };
#ifndef WIN32
    static constexpr auto expected_error_sun_family {
        "Invalid UNIX domain socket address"
    };
    static constexpr auto expected_error_sun_length_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of sun_len_type)"
    };
#endif
    static constexpr auto print_key_width {20};
    static constexpr auto print_value_width {10};

    static constexpr SocketHints unspec {
        AI_CANONNAME, AF_UNSPEC, SOCK_STREAM
    };

    static bool verbose {false};  // NOLINT

    auto create_sa(int family, std::size_t length = sa_size) -> sockaddr
    {
        sockaddr sa {};
#ifdef HAVE_SOCKADDR_SA_LEN
        sa.sa_len = length;
#else
        static_cast<void>(length);
#endif
        sa.sa_family = family;
        return sa;
    }

    auto create_sin(int family = AF_INET,
                    std::size_t length = sin_size) -> sockaddr_in
    {
        sockaddr_in sin {};
#ifdef HAVE_SOCKADDR_SA_LEN
        sin.sin_len = length;
#else
        static_cast<void>(length);
#endif
        sin.sin_family = family;
        return sin;
    }

    auto create_sin6(int family = AF_INET6,
                     std::size_t length = sin6_size) -> sockaddr_in6
    {
        sockaddr_in6 sin6 {};
#ifdef HAVE_SOCKADDR_SA_LEN
        sin6.sin6_len = length;
#else
        static_cast<void>(length);
#endif
        sin6.sin6_family = family;
        return sin6;
    }

    auto create_sun(int family = AF_UNIX,
                    std::size_t sun_length = sun_len_max,
                    std::size_t path_length = 0UL) -> sockaddr_un
    {
        sockaddr_un sun {};
#ifdef HAVE_SOCKADDR_SA_LEN
        sun.sun_len = sun_length;
#else
        static_cast<void>(sun_length);
#endif
        sun.sun_family = family;

        if (path_length != 0UL) {
            std::memset(&sun.sun_path, 'X', path_length);
        }

        return sun;
    }

    auto parse_arguments(int argc, char** argv) -> void
    {
        const auto [_, options] {parse(argc, argv, "v")};

        if (options.contains('?')) {
            std::cerr << "Usage: "
                      << *argv
                      << " [-v]"
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }

        if (options.contains('v')) {
            verbose = true;
        }

        static_cast<void>(_);
    }

    auto print(const Address& address) -> void
    {
        const auto length {address.length()};
        const auto family {address.family()};
        const auto port {address.port()};
        const auto size {address.size()};
        const auto text {address.text()};
        std::cout << "    "
                  << SocketFamily(family)
                  << std::endl
                  << std::setw(print_key_width) << "        Size: "
                  << std::right << std::setw(print_value_width) << size
                  << std::endl
                  << std::setw(print_key_width) << "        Length: "
                  << std::right << std::setw(print_value_width) << length
                  << std::endl
                  << std::setw(print_key_width) << "        Port: "
                  << std::right << std::setw(print_value_width) << port
                  << std::endl
                  << std::setw(print_key_width) << "        Address: "
                  << std::right << std::setw(print_value_width) << text
                  << std::endl;
    }

    auto print(const Error& error) -> void
    {
        if (verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    auto test_invalid_sa_family() -> void
    {
        std::string actual_error_str;

        try {
            const auto sa {create_sa(AF_UNSPEC)};
            validate(&sa, sizeof sa);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str == expected_error_sa_family);
    }

    auto test_invalid_sin_family() -> void
    {
        std::string actual_error_str;

        try {
            const auto sin {create_sin(AF_UNSPEC)};
            validate(&sin, sizeof sin);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str == expected_error_sa_family);
    }

    auto test_invalid_sin_length() -> void
    {
        std::string actual_error_str;

        try {
            const auto length {sin_size + 1};
            const auto sin {create_sin(AF_INET, length)};
            validate(&sin, length);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_sa_length_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_invalid_sin6_family() -> void
    {
        std::string actual_error_str;

        try {
            const auto sin6 {create_sin6(AF_UNSPEC)};
            validate(&sin6, sizeof sin6);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str == expected_error_sa_family);
    }

    auto test_invalid_sin6_length() -> void
    {
        std::string actual_error_str;

        try {
            const auto length {sin6_size + 1};
            const auto sin6 {create_sin6(AF_INET6, length)};
            validate(&sin6, length);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_sa_length_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

#ifndef WIN32

    auto test_invalid_sun_family() -> void
    {
        std::string actual_error_str;

        try {
            const auto sun {create_sun(AF_UNSPEC)};
            validate(&sun, sizeof sun);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str == expected_error_sun_family);
    }

    auto test_invalid_sun_length_large() -> void
    {
        std::string actual_error_str;

        try {
            const auto length {sun_len_max + 1};
            sockaddr_un sun {create_sun(AF_UNIX, length)};
            validate(&sun, length);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_sun_length_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_invalid_sun_length_small() -> void
    {
        std::string actual_error_str;

        try {
            const auto length {0UL};
            sockaddr_un sun {create_sun(AF_UNIX, length)};
            validate(&sun, length);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_sun_length_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_invalid_sun_path_large() -> void
    {
        std::string actual_error_str;

        try {
            const auto path_length {sun_size - sun_path_offset};
            const auto sun {create_sun(AF_UNIX, sun_len_max, path_length)};
            validate(&sun, sun_len_max);
            validate(to_bytestring(&sun, sun_len_max));
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_sun_length_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

#endif

    auto test_valid_sin(const Address& address) -> void
    {
        const auto family {address.family()};

        switch (family) {
        case AF_UNSPEC:
#ifndef WIN32
        case AF_UNIX:
#endif
        case AF_INET:
        case AF_INET6:
            break;
        default:
            assert(false);
        }

        const auto size {address.size()};
        const auto size_max {get_sa_size_maximum(family)};
        const auto size_min {get_sa_size_minimum(family)};

        if (!(size_min <= size && size <= size_max)) {
            assert(false);
        }

        const auto text {address.text()};

        switch (family) {
        case AF_INET:
            assert(text == "127.0.0.1");
            break;
        case AF_INET6:
            assert(text == "::1");
            break;
        default:
            assert(false);
        }
    }

    auto test_valid_sin() -> void
    {
        static const Hostname localhost {"localhost"};

        const auto hosts_result {get_hosts(localhost, unspec)};
        std::visit(Overloaded {
                [&](const HostVector& hosts) {
                    std::cout << "Socket addresses for "
                              << localhost
                              << ": "
                              << std::endl;

                    for (const auto& host : hosts) {
                        const Address address {host.address()};
                        print(address);
                        test_valid_sin(address);
                    }
                },
                    [&](const OsErrorResult& result) {
                        std::cout << "No "
                                  << localhost
                                  << " addresses: "
                                  << result.string()
                                  << std::endl;
                    }
                    }, hosts_result);
    }

#ifndef WIN32

    auto test_valid_sun_path_large() -> void
    {
        std::string actual_error_str;

        try {
            const auto path_length {sun_size - sun_path_offset - 1};
            const auto sun {create_sun(AF_UNIX, sun_len_max, path_length)};
            validate(&sun, sun_len_max);
            validate(to_bytestring(&sun, sun_len_max));
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.empty());
    }

    auto test_valid_sun_path_small() -> void
    {
        std::string actual_error_str;

        try {
            const auto sun {create_sun(AF_UNIX, sun_len_min)};
            validate(&sun, sun_len_min);
            validate(to_bytestring(&sun, sun_len_min));
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.empty());
    }

#endif

}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestAddress;

    try {
        const auto& context {Context::instance()};
        parse_arguments(argc, argv);

        if (verbose) {
            std::cout << context << std::endl;
        }

        test_valid_sin();
#ifndef WIN32
        test_valid_sun_path_large();
        test_valid_sun_path_small();
#endif
        test_invalid_sa_family();
        test_invalid_sin_family();
        test_invalid_sin_length();
        test_invalid_sin6_family();
        test_invalid_sin6_length();
#ifndef WIN32
        test_invalid_sun_family();
        test_invalid_sun_length_large();
        test_invalid_sun_length_small();
        test_invalid_sun_path_large();
#endif
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
