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

#include "network/assert.hpp"           // assert()
#include "network/network.hpp"          // Address, Error, Socket,
                                        // Symbol, TextBuffer,
                                        // create_socket, socket_error
#include "network/parse.hpp"            // parse()
#include "unix/connection.hpp"          // BUFFER_SIZE, SOCKET_HINTS,
                                        // SOCKET_NAME

#include <cstdio>       // std::perror()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::stoll(), std::string, std::to_string()

namespace
{
    using Network::Address;
    using Network::Error;
    using Network::Socket;
    using Network::Symbol;
    using Network::TextBuffer;
    using Network::UniqueSocket;
    using Network::create_socket;
    using Network::socket_error;

    using Number = long long;

    constexpr auto backlog_size {20};
    constexpr auto handle_width {6};
    constexpr auto indent_width {handle_width + 18};

    auto is_verbose {false};  // NOLINT

    auto accept(const Socket& as)
    {
        auto s {create_socket(as)};
        const Address host {s->get_name(Symbol::accept)};
        const Address peer {s->get_peername()};
        const Address self {s->get_sockname()};

        if (is_verbose) {
            // clang-format off
            std::cout << "Socket "
                      << std::right << std::setw(handle_width) << *s
                      << " connected "
                      << self
                      << std::endl
                      << std::right << std::setw(indent_width) << "to "
                      << peer
                      << std::endl;
            // clang-format on
        }

        assert(host.text() == peer.text());
        return s;
    }

    auto bind()
    {
        UniqueSocket s;

        if (auto result {bind(SOCKET_NAME, SOCKET_HINTS, is_verbose)}) {
            s.swap(result.value());
        }
        else {
            std::cerr << result.error().string() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        if (is_verbose) {
            const Address self {s->get_sockname()};
            // clang-format off
            std::cout << "Socket "
                      << std::right << std::setw(handle_width) << *s
                      << " bound to "
                      << self
                      << std::endl;
            // clang-format on
        }

        return s;
    }

    auto listen(const Socket& s)
    {
        if (const auto error {s.listen(backlog_size)}) {
            std::cerr << error.string() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        if (is_verbose) {
            // clang-format off
            std::cout << "Socket "
                      << std::right << std::setw(handle_width) << s
                      << " listening with backlog "
                      << backlog_size
                      << std::endl;
            // clang-format on
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

    auto read(const Socket& s) -> std::string
    {
        TextBuffer buffer {BUFFER_SIZE};
        const auto error {s.read(buffer)};

        if (error == socket_error) {
            std::perror("read");
            std::exit(EXIT_FAILURE);
        }

        return buffer;
    }

    auto write(const Socket& s, auto value)
    {
        const auto str {std::to_string(value)};
        const auto error {s.write(str)};

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
            const auto data_socket {accept(*connection_socket)};
            std::string str;
            Number sum {};

            while((str = read(*data_socket)) != "DOWN" && str != "END") {
                // Add received inputs.
                sum += std::stoll(str);
            }

            // Handle commands.
            if (str == "DOWN") {
                // Quit on DOWN command.
                shutdown_pending = true;
            }

            if (!shutdown_pending) {
                // Send output sum.
                write(*data_socket, sum);
            }
        }
    }
    catch (const Error& error) {
        std::cerr << error.what() << std::endl;
    }
}
