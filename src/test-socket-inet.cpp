// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/assert.hpp"                   // assert()
#include "network/inetsocket.hpp"               // InetSocket
#include "network/network.hpp"                  // Error, SocketData,
                                                // SocketHints,
                                                // UniqueSocket,
                                                // close(),
                                                // create_socket(),
                                                // family_type,
                                                // handle_null,
                                                // handle_type,
                                                // long_handle_type,
                                                // os_error_type,
                                                // run()
#include "network/parse.hpp"                    // parse()
#include "network/to-long-handle.hpp"           // to_long_handle()

#ifdef WIN32
#include <winsock2.h>   // AF_INET, AF_UNSPEC, SOCK_STREAM, ::close(),
                        // ::socket()
#else
#include <sys/socket.h> // AF_INET, AF_UNSPEC, SOCK_STREAM ::close(),
                        // ::socket()
#endif

#include <cstdlib>      // EXIT_FAILURE, std::exit(),
                        // std::size_t
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <sstream>      // std::ostringstream
#include <string>       // std::string
#include <utility>      // std::cmp_equal()

namespace
{
    using Network::Error;
    using Network::InetSocket;
    using Network::SharedRuntime;
    using Network::SocketData;
    using Network::SocketHints;
    using Network::UniqueSocket;
    using Network::create_socket;
    using Network::family_type;
    using Network::handle_null;
    using Network::handle_type;
    using Network::os_error_type;
    using Network::parse;
    using Network::long_handle_type;
    using Network::run;
    using Network::to_long_handle;

#ifdef WIN32
    constexpr auto expected_handle_null_re {
        ""
    };
#else
    constexpr auto expected_handle_null_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of .+)"
    };
#endif
    constexpr auto expected_invalid_re {
        R"(Null socket descriptor)"
    };
    constexpr auto expected_socket_re {
        R"(Call to ::socket\(.+\) failed with error \d+: .+)"
    };

    auto is_verbose {false}; // NOLINT

    auto parse_arguments(int argc, char** argv) -> void
    {
        [[maybe_unused]] const auto [args, options] {parse(argc, argv, "v")};

        if (options.contains('?')) {
            std::cerr << "Usage: "
                      << *argv
                      << " [-v]"
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }

        if (options.contains('v')) {
            is_verbose = true;
        }
    }

    auto print(const Error& error) -> void
    {
        if (is_verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    auto test(handle_type handle, const std::string& expected_re) -> void
    {
        std::string actual_str;

        try {
            assert(std::cmp_equal(to_long_handle(handle), handle));
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        if (expected_re.empty()) {
            assert(actual_str.empty());
        }
        else {
            const std::regex expected_regex {expected_re};
            assert(std::regex_match(actual_str, expected_regex));
        }
    }

    auto test(handle_type handle,
              family_type family,
              const SharedRuntime& sr,
              const std::string& expected_re) -> void
    {
        std::string actual_str;

        try {
            const SocketData sd {handle, family, sr};
            const InetSocket sock {sd};
            std::ostringstream oss;
            oss << sock;
            assert(static_cast<bool>(sock));
            assert(std::cmp_equal(static_cast<long_handle_type>(sock), handle));
            assert(oss.str() == std::to_string(static_cast<long_handle_type>(sock)));
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        if (expected_re.empty()) {
            assert(actual_str.empty());
        }
        else {
            const std::regex expected_regex {expected_re};
            assert(std::regex_match(actual_str, expected_regex));
        }
    }

    auto test(handle_type handle,
              family_type family,
              const std::string& expected_re) -> void
    {
        std::string actual_str;

        try {
            const auto sock {create_socket(handle, family, is_verbose)};
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        if (expected_re.empty()) {
            assert(actual_str.empty());
        }
        else {
            const std::regex expected_regex {expected_re};
            assert(std::regex_match(actual_str, expected_regex));
        }
    }

    auto test(const SocketHints& hints,
              const std::string& expected_re) -> void
    {
        std::string actual_str;

        try {
            const auto sock {create_socket(hints, is_verbose)};
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        if (expected_re.empty()) {
            assert(actual_str.empty());
        }
        else {
            const std::regex expected_regex {expected_re};
            assert(std::regex_match(actual_str, expected_regex));
        }
    }

    auto test_handle_null() -> void
    {
        test(handle_null, expected_handle_null_re);
    }

    auto test_handle_zero() -> void
    {
        test(0, "");
    }

    auto test_inet_socket_invalid(const SharedRuntime& sr) -> void
    {
        test(handle_null, AF_UNSPEC, sr, expected_invalid_re);
    }

    auto test_inet_socket_valid(const SharedRuntime& sr) -> void
    {
        const family_type family {AF_INET};
        const handle_type handle {::socket(family, SOCK_STREAM, 0)};
        const SocketData sd {handle, family, sr};
        test(handle, family, sr, "");
        Network::close(sd);
    }

    auto test_socket_family_invalid() -> void
    {
        const SocketHints hints {-1, SOCK_STREAM, 0};
        test(hints, expected_socket_re);
    }

    auto test_socket_handle_invalid() -> void
    {
        test(handle_null, AF_UNSPEC, expected_invalid_re);
    }

    auto test_socket_protocol_invalid() -> void
    {
        const SocketHints hints {AF_INET, SOCK_STREAM, -1};
        test(hints, expected_socket_re);
    }

    auto test_socket_socktype_invalid() -> void
    {
        const SocketHints hints {AF_INET, -1, 0};
        test(hints, expected_socket_re);
    }

    auto test_valid() -> void
    {
        const SocketHints hints {AF_INET, SOCK_STREAM, 0};
        test(hints, "");
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        parse_arguments(argc, argv);
        const auto rt {run(is_verbose)};

        if (is_verbose) {
            std::cout << *rt << std::endl;
        }

        test_handle_null();
        test_handle_zero();
        test_inet_socket_invalid(rt);
        test_inet_socket_valid(rt);
        test_socket_family_invalid();
        test_socket_handle_invalid();
        test_socket_protocol_invalid();
        test_socket_socktype_invalid();
        test_valid();
    }
    catch (const Error& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
