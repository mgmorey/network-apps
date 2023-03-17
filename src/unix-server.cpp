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

#include "network/network.h"            // Buffer, Socket, connect(),
                                        // read_string() socket_error,
                                        // to_bytestring(), write()
#include "network/parse.h"              // parse()
#include "unix-common.h"                // BUFFER_SIZE, SOCKET_NAME

#include <sys/socket.h>         // SOCK_SEQPACKET, ::accept(),
                                // ::listen()
#include <sys/un.h>             // AF_UNIX

#include <cerrno>       // EINVAL, EPROTONOSUPPORT
#include <cstdio>       // std::perror()
#include <cstdlib>      // EXIT_FAILURE, std::exit(), std::strtol()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <sstream>      // std::ostringstream
#include <string>       // std::string, std::to_string()

using Network::Pathname;
using Network::Socket;
using Network::SocketHints;
using Network::bind;
using Network::descriptor_type;
using Network::read_string;
using Network::socket_error;
using Network::to_bytestring;
using Network::write;

static constexpr auto backlog_size {20};
static constexpr auto radix {10};

static bool verbose {false};  // NOLINT

static constexpr auto expected_error_socket_re {
    R"(Call to ::socket[(][^)]+[)] failed with error \d+: .+)"
};

static auto parse(int argc, char** argv) -> void
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

static auto get_bind_socket(const SocketHints& hints) -> Socket
{
    static const Network::Socket sock {hints, true, verbose};
    return sock;
}

auto main(int argc, char* argv[]) -> int
{
    static constexpr SocketHints hints {0, AF_UNIX, SOCK_SEQPACKET, 0};

    try {
        // Fetch arguments from command line;
        parse(argc, argv);

        // Bind Unix domain socket to pathname.
        const auto bind_sock {get_bind_socket(hints)};
        const auto addr {to_bytestring(SOCKET_NAME)};
        const auto error {bind(bind_sock, addr, verbose)};
        const auto error_code {error.number()};
        bool shutdown_pending {false};

        if (error_code != 0) {
            std::cerr << error.string() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // Prepare for accepting connections. The backlog size is set to
        // 20. So while one request is being processed other requests can
        // be waiting.
        auto result = ::listen(descriptor_type {bind_sock}, backlog_size);

        if (result == -1) {
            std::perror("listen");
            std::exit(EXIT_FAILURE);
        }

        // This is the main loop for handling connections.
        while (!shutdown_pending) {
            // Wait for incoming connection.
            const Socket sock {
                ::accept(descriptor_type {bind_sock}, nullptr, nullptr),
                false,
                verbose
            };

            if (!sock.is_open()) {
                std::perror("accept");
                std::exit(EXIT_FAILURE);
            }

            long sum {0};

            while (true) {

                // Receive inputs.
                const auto [read_str, read_code] {read_string(BUFFER_SIZE, sock)};

                if (read_code == socket_error) {
                    perror("read");
                    exit(EXIT_FAILURE);
                }

                // Handle commands.

                if (read_str == "DOWN") {
                    // Quit on DOWN command.
                    shutdown_pending = true;
                    break;
                }

                if (read_str == "END") {
                    break;
                }

                // Add received inputs.
                sum += std::strtol(read_str.c_str(), nullptr, radix);
            }

            if (!shutdown_pending) {
                // Send output sum.
                const auto write_error {write(std::to_string(sum), sock)};

                if (write_error == -1) {
                    std::perror("write");
                    std::exit(EXIT_FAILURE);
                }
            }
        }
    }
    catch (const std::exception& error) {
        const std::regex expected_error_regex {expected_error_socket_re};

        if (!std::regex_match(error.what(), expected_error_regex)) {
            std::cerr << error.what() << std::endl;
        }
    }
}
