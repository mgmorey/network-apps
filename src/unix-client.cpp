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
                                        // write()
#include "network/parse.h"              // parse()
#include "unix-common.h"                // BUFFER_SIZE, SOCKET_NAME

#include <sys/socket.h>         // SOCK_SEQPACKET
#include <sys/un.h>             // AF_UNIX

#include <cerrno>       // EINVAL, EPROTONOSUPPORT
#include <cstdio>       // std::perror()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::string, std::to_string()

using Network::ArgumentSpan;
using Network::Socket;
using Network::socket_error;
using Network::write;

static bool verbose {false};  // NOLINT

namespace Client {
    auto connect() -> Socket
    {
        Socket sock {AF_UNIX, SOCK_SEQPACKET, 0, 0, false, verbose};
        const auto error {Network::connect(sock, SOCKET_NAME, verbose)};

        if (error) {
            std::cerr << error.string() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        return sock;
    }

    auto parse(int argc, char** argv) -> ArgumentSpan
    {
        const auto [operands, options] {Network::parse(argc, argv, "v")};

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
        const auto [read_str, read_error] {Network::read_string(size, sock)};

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
    const auto args {Client::parse(argc, argv)};

    try {
        // Connect Unix domain socket to pathname.
        const auto connect_sock {Client::connect()};
        bool shutdown_pending {false};

        // Send arguments to server.
        for (const auto& arg : args) {
            const std::string write_str {arg};

            if (write(write_str, connect_sock) == -1) {
                std::perror("write");
                break;
            }

            if (write_str == "DOWN") {
                shutdown_pending = true;
                break;
            }
        }

        if (!shutdown_pending) {
            // Request result.
            if (write("END", connect_sock) == -1) {
                std::perror("write");
                std::exit(EXIT_FAILURE);
            }

            // Receive result.
            const auto read_str {Client::read(connect_sock)};
            std::cerr << "Result: " << read_str << std::endl;
        }
    }
    catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
    }
}
