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

// This UNIX domain sequenced-packet socket example is adapted from the
// example in https://www.man7.org/linux/man-pages/man7/unix.7.html.

#include "network/network.hpp"          // Address, Socket,
                                        // UnixSocketHints,
                                        // socket_error,
                                        // to_bytestring()
#include "network/parse.hpp"            // parse()
#include "unix/connection.hpp"          // BUFFER_SIZE, SOCKET_HINTS,
                                        // SOCKET_NAME

#include <cstdio>       // std::perror()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::stoll(), std::string, std::to_string()
#include <utility>      // std::move()

namespace
{
    using Network::Address;
    using Network::Socket;
    using Network::TextBuffer;
    using Network::create_socket;
    using Network::socket_error;
    using Network::to_bytestring;

    using Number = long long;

    constexpr auto backlog_size {20};

    auto is_verbose {false};  // NOLINT

    auto accept_verbose(const Socket& bind_sock)
    {
        const auto [accept_data, accept_addr] {bind_sock.accept()};
        auto accept_sock {create_socket(accept_data)};

        if (accept_data.runtime()->is_verbose()) {
            std::cout << "Accepted connection from "
                      << Address(accept_addr)
                      << std::endl;
        }

        return accept_sock;
    }

    auto bind()
    {
        auto sock {create_socket(SOCKET_HINTS, is_verbose)};
        const auto bs {to_bytestring(SOCKET_NAME)};

        if (const auto result {sock->bind(bs)}) {
            std::cerr << result.string() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        return sock;
    }

    auto listen(const Socket& sock)
    {
        if (const auto result {sock.listen(backlog_size)}) {
            std::cerr << result.string() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    auto parse(int argc, char** argv)
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
            is_verbose = true;
        }
    }

    auto read(const Socket& sock) -> std::string
    {
        TextBuffer buffer {BUFFER_SIZE};
        const auto error {sock.read(buffer)};

        if (error == socket_error) {
            std::perror("read");
            std::exit(EXIT_FAILURE);
        }

        return buffer;
    }

    auto write(const Socket& sock, auto value)
    {
        const auto str {std::to_string(value)};
        const auto error {sock.write(str)};

        if (error == socket_error) {
            std::perror("write");
            std::exit(EXIT_FAILURE);
        }
    }
}

auto main(int argc, char* argv[]) -> int
{
    // Fetch arguments from command line.
    parse(argc, argv);

    try {
        // Bind Unix domain socket to pathname.
        const auto connection_socket {bind()};
        auto shutdown_pending {false};

        // Prepare for accepting connections. While one request is
        // being processed other requests can be waiting.
        listen(*connection_socket);

        // This is the main loop for handling connections.
        while (!shutdown_pending) {
            // Wait for incoming connection.
            const auto data_socket {accept_verbose(*connection_socket)};
            std::string read_str;
            Number sum {};

            while((read_str = read(*data_socket)) != "DOWN" &&
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
                write(*data_socket, sum);
            }
        }
    }
    catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
    }
}
