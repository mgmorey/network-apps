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

#include "network/argumentspan.h"       // ArgumentSpan
#include "network/network.h"            // Buffer, Socket, connect(),
                                        // read_string() socket_error,
                                        // to_bytestring(), write()
#include "network/parse.h"              // parse()
#include "unix-common.h"                // BUFFER_SIZE, SOCKET_NAME

#include <sys/socket.h>         // SOCK_SEQPACKET
#include <sys/un.h>             // AF_UNIX

#include <cerrno>       // EINVAL, EPROTONOSUPPORT
#include <cstdio>       // std::perror()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <string>       // std::string, std::to_string()

using Network::ArgumentSpan;
using Network::Pathname;
using Network::Socket;
using Network::SocketHints;
using Network::connect;
using Network::descriptor_type;
using Network::parse;
using Network::read_string;
using Network::socket_error;
using Network::to_bytestring;
using Network::write;

static bool verbose {false};  // NOLINT

static constexpr auto expected_error_socket_re {
    R"(Call to ::socket[(][^)]+[)] failed with error \d+: .+)"
};

namespace {
    auto get_connect_socket() -> Socket
    {
        static constexpr SocketHints hints {0, AF_UNIX, SOCK_SEQPACKET, 0};
        Socket sock {hints, false, verbose};
        return sock;
    }

    auto parse_arguments(int argc, char** argv) -> ArgumentSpan
    {
        const auto [operands, options] {parse(argc, argv, "v")};

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

        return operands;
    }

    auto read(const Socket& sock) -> std::string
    {
        static constexpr auto size {BUFFER_SIZE};
        const auto [read_str, read_error] {read_string(size, sock)};

        if (read_error == socket_error) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        return read_str;
    }
}

auto main(int argc, char* argv[]) -> int
{
    // Fetch arguments from command line.
    const auto args {parse_arguments(argc, argv)};

    try {
        // Connect Unix domain socket to pathname.
        const auto connect_sock {get_connect_socket()};
        const auto addr {to_bytestring(SOCKET_NAME)};
        const auto error {connect(connect_sock, addr, verbose)};

        if (error) {
            std::cerr << error.string() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        bool shutdown_pending {false};

        // Send arguments to server.
        for (const auto& arg : args) {
            const std::string write_str {arg};
            const auto write_code = write(write_str, connect_sock);

            if (write_code == -1) {
                std::perror("read");
                break;
            }

            if (write_str == "DOWN") {
                shutdown_pending = true;
                break;
            }
        }

        if (!shutdown_pending) {
            // Request result.
            const auto write_code {write("END", connect_sock)};

            if (write_code == -1) {
                std::perror("write");
                std::exit(EXIT_FAILURE);
            }

            // Receive result.
            const auto read_str {read(connect_sock)};
            std::cerr << "Result: " << read_str << std::endl;
        }
    }
    catch (const std::exception& error) {
        const std::regex expected_error_regex {expected_error_socket_re};

        if (!std::regex_match(error.what(), expected_error_regex)) {
            std::cerr << error.what() << std::endl;
        }
    }
}
