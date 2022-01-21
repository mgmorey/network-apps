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

#include "network/network.h"            // Buffer, Fd,
                                        // OptionalPathname, bind(),
                                        // close(), fd_null,
                                        // socket_error,
                                        // to_byte_string()
#include "unix-common.h"                // BUFFER_SIZE, SOCKET_NAME

#include <sys/socket.h>         // SOCK_SEQPACKET, ::accept(),
                                // ::listen(), ::socket()
#include <sys/un.h>             // AF_UNIX
#include <unistd.h>             // ::close(), ::read(), ::unlink(),
                                // ::write()

#include <cstdio>       // std::fprintf(), std::perror(),
                        // std::printf()
#include <cstdlib>      // std::atexit(), std::exit(), std::strtol()
#include <cstring>      // std::memset(), std::strcmp(),
                        // std::strcpy(), std::strncpy()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream
#include <string>       // std::string
#include <variant>      // std::get(), std::holds_alternative()

static constexpr auto backlog_size {20};
static constexpr auto radix {10};

static Network::fd_type sock;  // NOLINT

static auto clean_up() -> void
{
    // Close the socket.
    if (sock >= 0) {
        std::cerr << "Closing socket "
                  << sock
                  << std::endl;
        sock = Network::close(sock);
    }

    // Unlink the socket.
    std::cerr << "Removing file "
              << SOCKET_NAME
              << std::endl;
    ::unlink(SOCKET_NAME);
}

auto main() -> int
{
    Network::Buffer buffer(BUFFER_SIZE);
    bool shutdown {false};

    // Create local socket.
    sock = ::socket(AF_UNIX, SOCK_SEQPACKET, 0);

    if (sock == Network::fd_null) {
        std::perror("socket");
        std::exit(EXIT_FAILURE);
    }

    // Bind socket to socket name.
    const Network::OptionalPathname pathname {SOCKET_NAME};
    const auto sock_addr {Network::to_byte_string(pathname)};
    const auto sock_result {Network::bind(Network::Fd {sock}, sock_addr)};
    auto result {sock_result.number()};

    if (result == Network::socket_error) {
        std::perror("bind");
        std::exit(EXIT_FAILURE);
    }

    result = std::atexit(clean_up);

    if (result == -1) {
        std::perror("atexit");
        std::exit(EXIT_FAILURE);
    }

    // Prepare for accepting connections. The backlog size is set to
    // 20. So while one request is being processed other requests can
    // be waiting.
    result = ::listen(sock, backlog_size);

    if (result == -1) {
        std::perror("listen");
        std::exit(EXIT_FAILURE);
    }

    // This is the main loop for handling connections.
    for (;;) {
        int data_socket = 0;

        // Wait for incoming connection.
        data_socket = ::accept(sock, nullptr, nullptr);

        if (data_socket == -1) {
            std::perror("accept");
            std::exit(EXIT_FAILURE);
        }

        long sum {0};

        for (;;) {

            // Wait for next data packet.
            const auto size = ::read(data_socket, buffer.data(), buffer.size());

            if (size == -1) {
                std::perror("read");
                std::exit(EXIT_FAILURE);
            }

            // Handle commands.

            if (std::strncmp(buffer.data(), "DOWN", buffer.size()) == 0) {
                shutdown = true;
                break;
            }

            if (std::strncmp(buffer.data(), "END", buffer.size()) == 0) {
                break;
            }

            // Add received summand.
            const auto term {std::strtol(buffer.data(), nullptr, radix)};
            sum += term;
        }

        if (!shutdown) {
            // Send sum.
            std::ostringstream oss;
            oss << sum
                << '\0';
            std::string str {oss.str()};
            const auto size = ::write(data_socket, str.data(), str.size());

            if (size == -1) {
                std::perror("write");
                std::exit(EXIT_FAILURE);
            }
        }

        // Close socket.

        ::close(data_socket);

        // Quit on DOWN command.

        if (shutdown) {
            break;
        }
    }
}
