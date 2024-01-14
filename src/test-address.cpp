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
#include "network/network.h"            // Address, ByteString,
                                        // Context, Error, HostVector,
                                        // Hostname, OsErrorResult,
                                        // Overloaded, SocketFamily,
                                        // SocketHints, get_hosts(),
                                        // get_sa_family(),
                                        // get_sa_length(),
                                        // get_sa_size_maximum(),
                                        // get_sa_size_minimum(),
                                        // sa_len_max, to_bytestring()
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
#include <exception>    // std::exception
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <string>       // std::string
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
    using Network::Overloaded;
    using Network::SocketFamily;
    using Network::SocketHints;
    using Network::get_hosts;
    using Network::get_sa_family;
    using Network::get_sa_length;
    using Network::get_sa_size_maximum;
    using Network::get_sa_size_minimum;
    using Network::parse;
    using Network::to_bytestring;

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

    auto test_empty(const ByteString& addr) -> void
    {
        assert(addr.empty());
        assert(get_sa_family(addr) == AF_UNSPEC);
        assert(get_sa_length(addr, 0) == 0);
    }

    auto test_empty() -> void
    {
        test_empty({});
    }

    auto test_invalid_sa_family() -> void
    {
        std::string actual_error_str;

        try {
            sockaddr sa {};
            sa.sa_family = AF_UNSPEC;
            static_cast<void>(to_bytestring(&sa, sizeof sa));
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
            const sockaddr_in sin {};
            static_cast<void>(to_bytestring(&sin));
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
            sockaddr_in sin {};
            sin.sin_family = AF_INET;
            const void* ptr = &sin;
            const auto* sa = static_cast<const sockaddr*>(ptr);
            static_cast<void>(to_bytestring(sa, sizeof sin + 1));
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
            const sockaddr_in6 sin6 {};
            static_cast<void>(to_bytestring(&sin6));
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
            sockaddr_in6 sin6 {};
            sin6.sin6_family = AF_INET6;
            const void* ptr = &sin6;
            const auto* sa = static_cast<const sockaddr*>(ptr);
            static_cast<void>(to_bytestring(sa, sizeof sin6 + 1));
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
            const sockaddr_un sun {};
            static_cast<void>(to_bytestring(&sun, sizeof sun));
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str == expected_error_sun_family);
    }

    auto test_invalid_sun_length() -> void
    {
        std::string actual_error_str;

        try {
            sockaddr_un sun {};
            sun.sun_family = AF_UNIX;
            static_cast<void>(to_bytestring(&sun, sizeof sun + 1));
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_sun_length_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

#endif

    auto test_valid(const Address& address) -> void
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

    auto test_valid() -> void
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
                        test_valid(address);
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

        test_valid();
        test_empty();
        test_invalid_sa_family();
        test_invalid_sin_family();
        test_invalid_sin_length();
        test_invalid_sin6_family();
        test_invalid_sin6_length();
#ifndef WIN32
        test_invalid_sun_family();
        test_invalid_sun_length();
#endif
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
