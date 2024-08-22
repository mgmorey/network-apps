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

#include "network/assert.h"             // assert()
#include "network/network.h"            // Address, ByteString,
                                        // Context, Error, LogicError,
                                        // OsErrorResult, Socket,
                                        // os_error_type
#include "network/parse.h"              // parse()

#ifdef WIN32
#include <winsock2.h>   // AF_INET, SOCK_STREAM
#else
#include <sys/socket.h> // AF_INET, SOCK_STREAM
#endif

#include <cerrno>       // EACCES, ENOENT, EROFS
#include <cstdlib>      // EXIT_FAILURE, std::exit(),
                        // std::size_t
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <string>       // std::string

namespace TestSocket
{
    using Network::Context;
    using Network::Error;
    using Network::Socket;
    using Network::SocketHints;
    using Network::create;
    using Network::os_error_type;
    using Network::parse;
    using Network::socket_null;
    using Network::socket_type;

    static constexpr auto expected_error_socket_re {
        R"(Call to ::socket\(.+\) failed with error \d+: .+)"
    };

    static bool is_verbose {false};  // NOLINT

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
            is_verbose = true;
        }

        static_cast<void>(_);
    }

    auto print(const Error& error) -> void
    {
        if (is_verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    auto test_create(const SocketHints& hints,
                     const std::string& expected_error_re) -> void
    {
        std::string actual_error_str;

        try {
            Socket sock_1;
            const Socket sock_2 {sock_1};
            assert(!static_cast<bool>(sock_1));
            assert(!static_cast<bool>(sock_2));
            assert(static_cast<socket_type>(sock_1) == socket_null);
            assert(static_cast<socket_type>(sock_2) == socket_null);
            sock_1 = Socket {create(hints, is_verbose)};
            const Socket sock_3 {sock_1};
            assert(static_cast<bool>(sock_1));
            assert(static_cast<bool>(sock_3));
            assert(static_cast<socket_type>(sock_1) != socket_null);
            assert(static_cast<socket_type>(sock_3) != socket_null);
            assert(static_cast<socket_type>(sock_1) ==
                   static_cast<socket_type>(sock_3));
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        if (expected_error_re.empty()) {
            assert(actual_error_str.empty());
        }
        else {
            const std::regex expected_error_regex {expected_error_re};
            assert(std::regex_match(actual_error_str, expected_error_regex));
        }
    }

    auto test_create_hints_invalid_family() -> void
    {
        const SocketHints hints {-1, SOCK_STREAM, 0};
        test_create(hints, expected_error_socket_re);
    }

    auto test_create_hints_invalid_protocol() -> void
    {
        const SocketHints hints {AF_INET, SOCK_STREAM, -1};
        test_create(hints, expected_error_socket_re);
    }

    auto test_create_hints_invalid_type() -> void
    {
        const SocketHints hints {AF_INET, -1, 0};
        test_create(hints, expected_error_socket_re);
    }

    auto test_create_hints_valid() -> void
    {
        const SocketHints hints {AF_INET, SOCK_STREAM, 0};
        test_create(hints, "");
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestSocket;

    try {
        const auto& context {Context::instance()};
        parse_arguments(argc, argv);

        if (is_verbose) {
            std::cout << context << std::endl;
        }

        test_create_hints_valid();
        test_create_hints_invalid_family();
        test_create_hints_invalid_protocol();
        test_create_hints_invalid_type();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
