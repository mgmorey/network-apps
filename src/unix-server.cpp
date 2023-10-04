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

static constexpr auto backlog_size {20};

static bool verbose {false};  // NOLINT

namespace Server {
    auto accept(const Socket& sock) -> Socket
    {
        auto [accept_sock, accept_addr] {Network::accept(sock, verbose)};

        if (verbose) {
            std::cout << "Accepted connection from "
                      << Network::Address(accept_addr)
                      << std::endl;
        }

        return accept_sock;
    }

    auto bind() -> Socket
    {
        Socket bind_sock {AF_UNIX, SOCK_SEQPACKET, 0, 0, true, verbose};

        if (const auto error {Network::bind(bind_sock, SOCKET_NAME, verbose)}) {
            std::cerr << error.string() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        return bind_sock;
    }

    auto listen(const Socket& sock) -> int
    {
        const auto result {Network::listen(sock, backlog_size)};

        if (result == -1) {
            std::perror("listen");
            std::exit(EXIT_FAILURE);
        }

        return result;
    }

    auto parse(int argc, char** argv) -> void
    {
        const auto [_, options] {Network::parse(argc, argv, "v")};

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

    auto read(const Socket& sock) -> std::string
    {
        static constexpr auto size {BUFFER_SIZE};
        const auto [read_str, read_error] {Network::read_string(size, sock)};

        if (read_error == Network::socket_error) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        return read_str;
    }

    auto write(const Socket& sock, long long val) -> void
    {
        const auto write_str {std::to_string(val)};

        if (Network::write(write_str, sock) == -1) {
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
        static_cast<void>(Server::listen(bind_sock));

        // This is the main loop for handling connections.
        while (!shutdown_pending) {
            // Wait for incoming connection.
            const auto accept_sock {Server::accept(bind_sock)};
            std::string read_str;
            long long sum {0LL};

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
