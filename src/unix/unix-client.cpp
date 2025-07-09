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
                                        // create_socket,
                                        // socket_error,
                                        // to_bytestring()
#include "network/parse.hpp"            // parse()
#include "unix/connection.hpp"          // BUFFER_SIZE, SOCKET_HINTS,
                                        // SOCKET_NAME

#ifdef WIN32
#include <winsock2.h>       // SD_SEND
#else
#include <sys/socket.h>     // SHUT_WR
#endif

#include <cstdio>       // std::perror()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::string

namespace
{
    using Network::Address;
    using Network::create_socket;
    using Network::Error;
    using Network::Socket;
    using Network::TextBuffer;
    using Network::Symbol;
    using Network::handle_type;
    using Network::socket_error;
    using Network::to_bytestring;

    constexpr auto handle_width {6};
    constexpr auto indent_width {handle_width + 18};
#ifdef WIN32
    constexpr auto shutdown_flags {SD_SEND};
#else
    constexpr auto shutdown_flags {SHUT_WR};
#endif

    auto is_verbose {false};  // NOLINT

    auto connect()
    {
        auto ps {create_socket(SOCKET_HINTS, is_verbose)};

        if (const auto error {ps->connect(to_bytestring(SOCKET_NAME))}) {
            std::cerr << error.string() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        const Address host {ps->name(Symbol::connect)};
        const Address peer {ps->name(Symbol::getpeername)};
        const Address self {ps->name(Symbol::getsockname)};

        if (is_verbose) {
            // clang-format off
            std::cout << "Socket "
                      << std::right << std::setw(handle_width) << *ps
                      << " connected "
                      << self
                      << std::endl
                      << std::right << std::setw(indent_width) << "to "
                      << peer
                      << std::endl;
            // clang-format on
        }

        assert(host.text() == peer.text());
        return ps;
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

    auto shutdown(const Socket& s)
    {
        if (auto error = s.shutdown(shutdown_flags)) {
            std::cerr << error.string() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    auto write(const std::string& str, const Socket& s)
    {
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
    const auto args {parse(argc, argv)};

    try {
        // Connect Unix domain socket to pathname.
        auto s {connect()};
        auto shutdown_pending {false};

        // Send arguments to server.
        for (const auto& arg : args) {
            const std::string write_str {arg};
            write(write_str, *s);

            if (write_str == "DOWN") {
                shutdown_pending = true;
                shutdown(*s);
                break;
            }
        }

        if (!shutdown_pending) {
            // Request result.
            write("END", *s);

            // Receive result.
            const auto read_str {read(*s)};
            std::cout << "Result: " << read_str << std::endl;
        }
    }
    catch (const Error& error) {
        std::cerr << error.what() << std::endl;
    }
}
