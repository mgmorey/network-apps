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
                                        // OptionalPathname, close(),
                                        // connect(), socket_error,
                                        // to_byte_string()
#include "unix-common.h"                // BUFFER_SIZE, SOCKET_NAME

#include <sys/socket.h>         // SOCK_SEQPACKET, ::connect(),
                                // ::socket()
#include <sys/un.h>             // AF_UNIX
#include <unistd.h>             // ::close(), ::read(), ::write()

#include <cerrno>       // errno
#include <cstdio>       // std::fprintf(), std::perror(),
                        // std::printf()
#include <cstdlib>      // std::exit()
#include <cstring>      // std::memset(), std::strcmp(),
                        // std::strcpy(), std::strncpy()
#include <iostream>     // std::cerr, std::endl
#include <span>         // std::span

auto main(int argc, char *argv[]) -> int
{
    bool shutdown {false};

    // Create local socket.
    Network::fd_type sock {::socket(AF_UNIX, SOCK_SEQPACKET, 0)};

    if (sock == -1) {
        std::perror("socket");
        std::exit(EXIT_FAILURE);
    }

    // Connect socket to socket address.
    const Network::OptionalPathname pathname {SOCKET_NAME};
    const auto sock_addr {Network::to_byte_string(pathname)};
    const auto sock_result {Network::connect(Network::Fd {sock}, sock_addr)};
    auto connect_result {sock_result.number()};

    if (connect_result == Network::socket_error) {
        std::cerr << "Service is unavailable"
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Send arguments.

    const auto args = std::span(argv, size_t(argc));

    for (int i = 1; i < argc; ++i) {
        const auto result {::write(sock,
                                   args[i],
                                   std::strlen(args[i]) + 1)};

        if (result == -1) {
            std::perror("write");
            break;
        }

        if (std::strcmp(args[1], "DOWN") == 0) {
            shutdown = true;
            break;
        }
    }

    if (!shutdown) {
        Network::Buffer buffer(BUFFER_SIZE);

        // Request result.
        std::strncpy(buffer.data(), "END", buffer.size());
        auto result {::write(sock,
                             buffer.data(),
                             std::strlen(buffer.data()) + 1)};

        if (result == -1) {
            std::perror("write");
            std::exit(EXIT_FAILURE);
        }

        // Receive result.
        result = ::read(sock, buffer.data(), buffer.size());

        if (result == -1) {
            std::perror("read");
            std::exit(EXIT_FAILURE);
        }

        std::cerr << "Result: "
                  << buffer
                  << std::endl;
    }

    // Close socket.
    ::close(sock);
}
