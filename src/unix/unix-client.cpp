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

#include "network/argumentspan.hpp"     // ArgumentSpan
#include "network/network.hpp"          // Error, Socket,
                                        // socket_error,
                                        // to_bytestring()
#include "network/parse.hpp"            // parse()
#include "unix/connection.hpp"          // BUFFER_SIZE, SOCKET_HINTS,
                                        // SOCKET_NAME

#include <cstdio>       // std::perror()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::string, std::to_string()

namespace
{
    using Network::ArgumentSpan;
    using Network::Error;
    using Network::Socket;
    using Network::TextBuffer;
    using Network::create_socket;
    using Network::socket_error;
    using Network::to_bytestring;

    auto is_verbose {false};  // NOLINT

    auto connect()
    {
        auto sock {create_socket(SOCKET_HINTS, is_verbose)};
        const auto bs {to_bytestring(SOCKET_NAME)};

        if (const auto error {sock->connect(bs)}) {
            std::cerr << error.string() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        return sock;
    }

    auto parse(int argc, char** argv)
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
            is_verbose = true;
        }

        return operands;
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

    auto write(const std::string& str, const Socket& sock)
    {
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
    const auto args {parse(argc, argv)};

    try {
        // Connect Unix domain socket to pathname.
        auto data_socket {connect()};
        auto shutdown_pending {false};

        // Send arguments to server.
        for (const auto& arg : args) {
            const std::string write_str {arg};
            write(write_str, *data_socket);

            if (write_str == "DOWN") {
                shutdown_pending = true;
                break;
            }
        }

        if (!shutdown_pending) {
            // Request result.
            write("END", *data_socket);

            // Receive result.
            const auto read_str {read(*data_socket)};
            std::cout << "Result: " << read_str << std::endl;
        }
    }
    catch (const Error& error) {
        std::cerr << error.what() << std::endl;
    }
}
