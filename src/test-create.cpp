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
#include <winsock2.h>   // AF_INET, IPPROTO_IP, IPPROTO_TCP
#else
#include <netinet/in.h> // IPPROTO_IP, IPPROTO_TCP
#include <sys/socket.h> // AF_INET
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
    using Network::OsErrorResult;
    using Network::Socket;
    using Network::SocketHints;
    using Network::os_error_type;
    using Network::parse;

    static constexpr auto expected_error_socket_re {
        R"(Call to ::socket\(.+\) failed with error \d+: .+)"
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

    auto print(const Error& error) -> void
    {
        if (verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    auto test_socket(const SocketHints& hints,
                     const std::string& expected_error_re) -> void
    {
        std::string actual_error_str;

        try {
            const Socket sock {hints, verbose};
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

    auto test_socket_invalid_protocol() -> void
    {
        const SocketHints hints {AF_INET, SOCK_STREAM, -1};
        test_socket(hints, expected_error_socket_re);
    }

    auto test_socket_invalid_type() -> void
    {
        const SocketHints hints {AF_INET, 0, 0};
        test_socket(hints, expected_error_socket_re);
    }

    auto test_socket_valid() -> void
    {
        const SocketHints hints {AF_INET, SOCK_STREAM, 0};
        test_socket(hints, "");
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestSocket;

    try {
        const auto& context {Context::instance()};
        parse_arguments(argc, argv);

        if (verbose) {
            std::cout << context << std::endl;
        }

        test_socket_valid();
        test_socket_invalid_protocol();
        test_socket_invalid_type();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
