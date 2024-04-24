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

#include "network/network.h"            // Address, Socket, accept(),
                                        // bind(), listen(),
                                        // read_string(),
                                        // socket_error, write()
#include "network/parse.h"              // parse()
#include "unix-common.h"                // BUFFER_SIZE, SOCKET_NAME

#include <sys/socket.h>         // SOCK_SEQPACKET
#include <sys/un.h>             // AF_UNIX

#include <cstdio>       // std::perror()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::stoll(), std::string, std::to_string()

using Network::Socket;
using Network::SocketHints;
using Network::af_unix;
using Network::socket_error;

using Number = long long;

static constexpr auto backlog_size {20};

static bool verbose {false};  // NOLINT

namespace Server {
    auto accept(const Socket& bind_sock) -> Socket
    {
        auto [accept_sock, accept_addr] {Network::accept(bind_sock, verbose)};

        if (verbose) {
            std::cout << "Accepted connection from "
                      << Network::Address(accept_addr)
                      << std::endl;
        }

        return accept_sock;
    }

    auto bind() -> Socket
    {
        Socket sock {SocketHints {af_unix, SOCK_SEQPACKET}, verbose};

        if (const auto result {Network::bind(sock, SOCKET_NAME, verbose)}) {
            std::cerr << result.string() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        return sock;
    }

    auto listen(const Socket& sock) -> void
    {
        const auto result {Network::listen(sock, backlog_size)};

        if (result == socket_error) {
            std::perror("listen");
            std::exit(EXIT_FAILURE);
        }
    }

    auto parse(int argc, char** argv) -> void
    {
        const auto arguments {Network::parse(argc, argv, "v")};
        const auto& options {arguments.second};

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
    }

    auto read(const Socket& sock) -> std::string
    {
        static constexpr auto size {BUFFER_SIZE};
        const auto [str, error] {Network::read_string(size, sock)};

        if (error == socket_error) {
            std::perror("read");
            std::exit(EXIT_FAILURE);
        }

        return str;
    }

    auto write(const Socket& sock, auto value) -> void
    {
        const auto str {std::to_string(value)};
        const auto error {Network::write(str, sock)};

        if (error == socket_error) {
            std::perror("write");
            std::exit(EXIT_FAILURE);
        }
    }
}

auto main(int argc, char* argv[]) -> int
{
    // Fetch arguments from command line.
    Server::parse(argc, argv);

    try {
        // Bind Unix domain socket to pathname.
        const auto bind_sock {Server::bind()};
        bool shutdown_pending {false};

        // Prepare for accepting connections. While one request is
        // being processed other requests can be waiting.
        Server::listen(bind_sock);

        // This is the main loop for handling connections.
        while (!shutdown_pending) {
            // Wait for incoming connection.
            const auto accept_sock {Server::accept(bind_sock)};
            std::string read_str;
            Number sum {};

            while((read_str = Server::read(accept_sock)) != "DOWN" &&
                  read_str != "END") {
                // Add received inputs.
                sum += std::stoll(read_str);
            }

            // Handle commands.
            if (read_str == "DOWN") {
                // Quit on DOWN command.
                shutdown_pending = true;
            }

            if (!shutdown_pending) {
                // Send output sum.
                Server::write(accept_sock, sum);
            }
        }
    }
    catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
    }
}
