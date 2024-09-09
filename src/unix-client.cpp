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
#include "network/network.h"            // Socket, create(), connect(),
                                        // read_string() socket_error,
                                        // write()
#include "network/parse.h"              // parse()
#include "unix-common.h"                // BUFFER_SIZE, SOCKET_NAME

#include <sys/socket.h>         // SOCK_SEQPACKET
#include <sys/un.h>             // AF_UNIX

#include <cstdio>       // std::perror()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::string, std::to_string()

using Network::ArgumentSpan;
using Network::SharedSocket;
using Network::UnixSocketHints;
using Network::create;
using Network::socket_error;

static auto is_verbose {false};  // NOLINT

namespace Client {
    auto connect() -> SharedSocket
    {
        const UnixSocketHints hints {SOCK_SEQPACKET};
        auto sock {create(hints, is_verbose)};

        if (const auto error {sock->connect(SOCKET_NAME)}) {
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
            is_verbose = true;
        }

        return operands;
    }

    auto read(const SharedSocket& sock) -> std::string
    {
        static constexpr auto size {BUFFER_SIZE};
        const auto [str, error] {Network::read_string(sock, size)};

        if (error == socket_error) {
            std::perror("read");
            std::exit(EXIT_FAILURE);
        }

        return str;
    }

    auto write(const std::string& str, const SharedSocket& sock) -> void
    {
        const auto error {Network::write(sock, str)};

        if (error == socket_error) {
            std::perror("write");
            std::exit(EXIT_FAILURE);
        }
    }
}

auto main(int argc, char* argv[]) -> int
{
    // Fetch arguments from command line.
    const auto args {Client::parse(argc, argv)};

    try {
        // Connect Unix domain socket to pathname.
        const auto sock {Client::connect()};
        auto shutdown_pending {false};

        // Send arguments to server.
        for (const auto& arg : args) {
            const std::string write_str {arg};
            Client::write(write_str, sock);

            if (write_str == "DOWN") {
                shutdown_pending = true;
                break;
            }
        }

        if (!shutdown_pending) {
            // Request result.
            Client::write("END", sock);

            // Receive result.
            const auto read_str {Client::read(sock)};
            std::cerr << "Result: " << read_str << std::endl;
        }
    }
    catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
    }
}
