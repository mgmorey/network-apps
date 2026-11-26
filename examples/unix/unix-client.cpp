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

// This UNIX domain socket example is adapted from the example in
// https://www.man7.org/linux/man-pages/man7/unix.7.html.

#include "network/network.hpp"          // Address, Error, Socket,
                                        // UniqueSocket, connect()
#include "connection.hpp"               // NetworkExamples::Unix
#include "network/parse.hpp"            // parse()

#ifdef _WIN32
#include <winsock2.h>       // SD_SEND
#else
#include <sys/socket.h>     // SHUT_WR
#endif

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <cstring>      // std::strcmp()
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::string
#include <utility>      // std::move()

namespace
{
    using Network::Address;
    using Network::Error;
    using Network::Socket;
    using Network::UniqueSocket;
    using Network::connect;

    constexpr auto handle_width {6};
    constexpr auto indent_width {handle_width + 18};
#ifdef _WIN32
    constexpr auto shutdown_flags {SD_SEND};
#else
    constexpr auto shutdown_flags {SHUT_WR};
#endif

    auto is_verbose {false};  // NOLINT

    auto connect()
    {
        auto result {connect(NetworkExamples::Unix::socket_name,
                             NetworkExamples::Unix::socket_hints,
                             is_verbose),};

        if (!result) {
            std::cerr << result.error().string() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        auto& s {*result};
        const Address peer {s->get_peername()};

        if (is_verbose) {
            const Address self {s->get_sockname()};
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

        return std::move(s);
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
        return NetworkExamples::Unix::read_message(s);
    }

    auto shutdown(const Socket& s)
    {
        if (const auto error = s.shutdown(shutdown_flags)) {
            std::cerr << error.string() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    auto write(const std::string& str, const Socket& s)
    {
        NetworkExamples::Unix::write_message(s, str);
    }
}

auto main(int argc, char* argv[]) -> int
{
    // Fetch arguments from command line.
    const auto args {parse(argc, argv)};

    try {
        // Connect Unix domain socket to pathname.
        const auto s {connect()};
        auto shutdown_pending {false};

        // Send arguments to server.
        for (const auto& arg : args) {
            write(arg, *s);

            if (std::strcmp(arg, "DOWN") == 0) {
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
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
