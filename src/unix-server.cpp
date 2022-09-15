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

#include "network/commandline.h"        // CommandLine
#include "network/network.h"            // Buffer, Fd, connect(),
                                        // socket_error,
                                        // to_byte_string()
#include "network/to-size.h"            // to_size()
#include "unix-common.h"                // BUFFER_SIZE, SOCKET_NAME

#include <sys/socket.h>         // SOCK_SEQPACKET, ::accept(),
                                // ::listen(), ::socket()
#include <sys/types.h>          // ssize_t
#include <sys/un.h>             // AF_UNIX
#include <unistd.h>             // ::read(), ::write()

#include <cerrno>       // EINVAL, EPROTONOSUPPORT
#include <cstdio>       // std::perror()
#include <cstdlib>      // std::exit(), std::strtol()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <span>         // std::span
#include <sstream>      // std::ostringstream
#include <string>       // std::string, std::to_string()

using Network::Buffer;
using Network::CommandLine;
using Network::Fd;
using Network::Pathname;
using Network::SocketHints;
using Network::bind;
using Network::fd_type;
using Network::format_os_error;
using Network::socket_error;
using Network::to_byte_string;
using Network::to_size;

using IoResult = std::pair<std::string, ssize_t>;

static constexpr auto backlog_size {20};
static constexpr auto radix {10};

static bool verbose {false};  // NOLINT

static auto format_message(int error) -> std::string
{
    std::ostringstream oss;
    oss << ("Call to ::socket(domain=AF_UNIX, type=SOCK_SEQPACKET, protocol=0) "
            "failed with error ")
        << error
        << ": "
        << format_os_error(error);
    return oss.str();
}

static auto parse_arguments(CommandLine& command_line) -> void
{
    int opt {};

    while ((opt = command_line.option()) != -1) {
        switch (opt) {
        case 'v':
            verbose = true;
            break;
        case '?':
            std::cerr << "Usage: "
                      << *command_line.argument(0)
                      << " [-v]"
                      << std::endl;
            std::exit(EXIT_FAILURE);
        default:
            abort();
        }
    }
}

static auto get_bind_socket(const SocketHints& hints) -> Fd
{
    static Network::Fd fd {hints, true, verbose};
    return fd;
}

static auto read(const Fd& fd) -> IoResult
{
    Buffer buffer {BUFFER_SIZE};
    return {buffer, ::read(fd_type {fd},
                           buffer.data(),
                           buffer.size())};
}

static auto write(const std::string& str, const Fd& fd) -> ssize_t
{
    return ::write(fd_type {fd}, str.data(), str.size());
}

auto main(int argc, char* argv[]) -> int
{
    static constexpr SocketHints hints {0, AF_UNIX, SOCK_SEQPACKET, 0};

    try {
        // Fetch arguments from command line;
        CommandLine command_line(argc, argv, "v");
        parse_arguments(command_line);

        // Bind Unix domain socket to pathname.
        const auto bind_fd {get_bind_socket(hints)};
        const auto addr {to_byte_string(SOCKET_NAME)};
        const auto error {bind(bind_fd, addr, verbose)};
        const auto error_code {error.number()};

        if (error_code != 0) {
            std::cerr << error.string()
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // Prepare for accepting connections. The backlog size is set to
        // 20. So while one request is being processed other requests can
        // be waiting.
        auto result = ::listen(fd_type {bind_fd}, backlog_size);

        if (result == -1) {
            std::perror("listen");
            std::exit(EXIT_FAILURE);
        }

        bool shutdown {false};

        // This is the main loop for handling connections.
        while (!shutdown) {
            // Wait for incoming connection.
            Fd fd {
                ::accept(fd_type {bind_fd},
                         nullptr,
                         nullptr),
                false,
                verbose
            };

            if (!fd) {
                std::perror("accept");
                std::exit(EXIT_FAILURE);
            }

            long sum {0};

            while (true) {

                // Receive inputs.
                const auto [read_str, read_code] {read(fd)};

                if (read_code == socket_error) {
                    perror("read");
                    exit(EXIT_FAILURE);
                }

                // Handle commands.

                if (read_str == "DOWN") {
                    // Quit on DOWN command.
                    shutdown = true;
                    break;
                }

                if (read_str == "END") {
                    break;
                }

                // Add received inputs.
                sum += std::strtol(read_str.c_str(), nullptr, radix);
            }

            if (!shutdown) {
                // Send output sum.
                const auto write_error {write(std::to_string(sum), fd)};

                if (write_error == -1) {
                    std::perror("write");
                    std::exit(EXIT_FAILURE);
                }
            }
        }
    }
    catch (const std::exception& error) {
        if (error.what() != format_message(EINVAL) &&
            error.what() != format_message(EPROTONOSUPPORT)) {
            std::cerr << error.what()
                      << std::endl;
        }
    }
}
