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
                                        // OsErrorResult,
                                        // SocketFamily, SocketHints,
                                        // get_hosts(),
                                        // get_sa_size_maximum(),
                                        // get_sa_size_minimum(),
                                        // sa_family_type, sa_size,
                                        // sin_family_type,
                                        // sun_len_max, sun_len_min,
                                        // sun_path_offset, sun_size,
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
                            // sockaddr, sockaddr_in6, sockaddr_in,
                            // sockaddr_un
#endif

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <cstring>      // std::memset()
#include <exception>    // std::exception
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <string>       // std::string
#include <type_traits>  // std::decay_t, std::is_same_v
#include <variant>      // std::visit()

namespace TestAddress
{
    using Network::Address;
    using Network::ByteString;
    using Network::Context;
    using Network::Error;
    using Network::HostVector;
    using Network::Hostname;
    using Network::OsErrorResult;
    using Network::SocketFamily;
    using Network::SocketHints;
    using Network::always_false_v;
    using Network::get_hosts;
    using Network::get_sa_size_maximum;
    using Network::get_sa_size_minimum;
    using Network::parse;
#ifndef WIN32
    using Network::path_length_max;
#endif
    using Network::sa_family_type;
    using Network::sa_size;
    using Network::sin_family_type;
#ifndef WIN32
    using Network::sun_len_max;
    using Network::sun_len_min;
    using Network::sun_path_offset;
    using Network::sun_size;
#endif
    using Network::to_bytestring;
    using Network::validate;

    static constexpr auto expected_error_family_re {
        R"(Invalid socket address family: (\d+))"
    };
    static constexpr auto expected_error_length_re {
        R"(Value (\d+) is out of range \[\d+, \d+\] of \w+_type)"
    };
    static constexpr auto print_key_width {20};
    static constexpr auto print_value_width {10};

    static const SocketHints unspec {AF_UNSPEC, SOCK_STREAM, 0, AI_CANONNAME};

    static bool verbose {false};  // NOLINT

    auto create_sa(sa_family_type family, std::size_t length = sa_size) -> sockaddr
    {
        sockaddr sa{};
#ifdef HAVE_SOCKADDR_SA_LEN
        sa.sa_len = length;
#else
        static_cast<void>(length);
#endif
        sa.sa_family = family;
        return sa;
    }

    auto create_sin(sin_family_type family = AF_INET) -> sockaddr_in
    {
        sockaddr_in sin {};
#ifdef HAVE_SOCKADDR_SA_LEN
        sin.sin_len = sizeof sin;
#endif
        sin.sin_family = family;
        return sin;
    }

    auto create_sin6(sin_family_type family = AF_INET6) -> sockaddr_in6
    {
        sockaddr_in6 sin6 {};
#ifdef HAVE_SOCKADDR_SA_LEN
        sin6.sin6_len = sizeof sin6;
#endif
        sin6.sin6_family = family;
        return sin6;
    }

#ifndef WIN32

    auto create_sun(sa_family_type family = AF_UNIX,
                    std::size_t sun_len = sun_size,
                    std::size_t path_len = 0UL) -> sockaddr_un
    {
        sockaddr_un sun {};
#ifdef HAVE_SOCKADDR_SA_LEN
        sun.sun_len = sun_len;
#else
        static_cast<void>(sun_len);
#endif
        sun.sun_family = family;

        if (path_len != 0UL) {
            std::memset(&sun.sun_path, 'X', path_len);
        }

        return sun;
    }

#endif

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

    auto print(const Address& address, std::size_t size) -> void
    {
        const auto family {address.family()};
#ifdef HAVE_SOCKADDR_SA_LEN
        const auto length {address.length()};
#endif
        const auto port {address.port()};
        const auto text {address.text()};
        std::cout << "    "
                  << SocketFamily(family)
                  << std::endl
                  << std::setw(print_key_width) << "        Size: "
                  << std::right << std::setw(print_value_width) << size
                  << std::endl
#ifdef HAVE_SOCKADDR_SA_LEN
                  << std::setw(print_key_width) << "        Length: "
                  << std::right << std::setw(print_value_width) << length
                  << std::endl
#endif
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

    auto test_sa(const sockaddr& sa, std::size_t sa_len,
                 const std::string& expected_error_re) -> void
    {
        const std::regex expected_error_regex {expected_error_re};
        std::string actual_error_str;

        try {
            validate(&sa, sa_len);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(std::regex_match(actual_error_str, expected_error_regex));
        actual_error_str.clear();

        try {
            validate(to_bytestring(&sa, sa_len));
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_sa_invalid_length() -> void
    {
        const auto sa {create_sa(AF_UNSPEC, 0)};
        test_sa(sa, 0, expected_error_length_re);
    }

    auto test_sin(const sockaddr_in& sin,
                  const std::string& expected_error_re) -> void
    {
        const std::regex expected_error_regex {expected_error_re};
        std::string actual_error_str;

        try {
            validate(&sin);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(std::regex_match(actual_error_str, expected_error_regex));

        if (sin.sin_family == AF_INET) {
            actual_error_str.clear();

            try {
                validate(to_bytestring(&sin, sizeof sin));
            }
            catch (const Error& error) {
                print(error);
                actual_error_str = error.what();
            }

            assert(std::regex_match(actual_error_str, expected_error_regex));
        }
    }

    auto test_sin_invalid_family() -> void
    {
        const auto sin {create_sin(AF_UNSPEC)};
        test_sin(sin, expected_error_family_re);
    }

    auto test_sin6(const sockaddr_in6 &sin6,
                   const std::string& expected_error_re) -> void
    {
        const std::regex expected_error_regex {expected_error_re};
        std::string actual_error_str;

        try {
            validate(&sin6);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(std::regex_match(actual_error_str, expected_error_regex));

        if (sin6.sin6_family == AF_INET6) {
            actual_error_str.clear();

            try {
                validate(to_bytestring(&sin6, sizeof sin6));
            }
            catch (const Error& error) {
                print(error);
                actual_error_str = error.what();
            }

            assert(std::regex_match(actual_error_str, expected_error_regex));
        }
    }

    auto test_sin6_invalid_family() -> void
    {
        const auto sin6 {create_sin6(AF_UNSPEC)};
        test_sin6(sin6, expected_error_family_re);
    }

#ifndef WIN32

    auto test_char(const char* path,
                   const std::string& expected_error_re) -> void
    {
        const std::regex expected_error_regex {expected_error_re};
        std::string actual_error_str;

        try {
            validate(path);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_char_invalid_length() -> void
    {
        const std::string path(path_length_max + 1, '.');
        test_char(path.c_str(), expected_error_length_re);
    }

    auto test_path(const std::string& path,
                   const std::string& expected_error_re) -> void
    {
        const std::regex expected_error_regex {expected_error_re};
        std::string actual_error_str;

        try {
            validate(path);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_path_invalid_length() -> void
    {
        const std::string path(path_length_max + 1, '.' );
        test_path(path, expected_error_length_re);
    }

    auto test_sun(const sockaddr_un& sun, std::size_t sun_len,
                  const std::string& expected_error_re) -> void
    {
        const std::regex expected_error_regex {expected_error_re};
        std::string actual_error_str;

        try {
            validate(&sun, sun_len);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(std::regex_match(actual_error_str, expected_error_regex));

        if (sun.sun_family == AF_UNIX) {
            actual_error_str.clear();

            try {
                validate(to_bytestring(&sun, sun_len));
            }
            catch (const Error& error) {
                print(error);
                actual_error_str = error.what();
            }

            assert(std::regex_match(actual_error_str, expected_error_regex));
        }
    }

    auto test_sun_invalid_family() -> void
    {
        const auto sun {create_sun(AF_UNSPEC, sun_len_min)};
        test_sun(sun, sun_len_min, expected_error_family_re);
    }

    auto test_sun_valid_path_large() -> void
    {
        const auto path_len {sun_size - sun_path_offset};
        const auto sun {create_sun(AF_UNIX, sun_len_max, path_len)};
        test_sun(sun, sun_len_max, {});
    }

    auto test_sun_valid_path_small() -> void
    {
        const auto sun {create_sun(AF_UNIX, sun_len_min)};
        test_sun(sun, sun_len_min, {});
    }

#endif

    auto test_valid(const Address& address, std::size_t size) -> void
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

    auto test_valid() -> void
    {
        static const Hostname localhost {"localhost"};

        const auto hosts_result {get_hosts(localhost, unspec)};
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, HostVector>) {
                std::cout << "Socket addresses for "
                          << localhost
                          << ": "
                          << std::endl;

                for (const auto& host : arg) {
                    const ByteString& addr {host.address()};
                    const Address address {addr};
                    print(address, addr.size());
                    test_valid(address, addr.size());
                }
            }
            else if constexpr (std::is_same_v<T, OsErrorResult>) {
                std::cout << "No "
                          << localhost
                          << " addresses: "
                          << arg.string()
                          << std::endl;
            }
            else {
                static_assert(always_false_v<T>, VISITOR_ERROR);
            }
        }, hosts_result);
    }
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

        test_sa_invalid_length();
        test_sin6_invalid_family();
        test_sin_invalid_family();
#ifndef WIN32
        test_char_invalid_length();
        test_path_invalid_length();
        test_sun_invalid_family();
        test_sun_valid_path_large();
        test_sun_valid_path_small();
#endif
        test_valid();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
