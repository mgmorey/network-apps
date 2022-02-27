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

#include "network/network.h"            // Buffer, Fd, close(),
                                        // connect(), socket_error,
                                        // to_byte_string()
#include "unix-common.h"                // BUFFER_SIZE, SOCKET_NAME

#include <sys/socket.h>         // SOCK_SEQPACKET
#include <sys/un.h>             // AF_UNIX
#include <unistd.h>             // ::read(), ::write()

#include <cstdio>       // std::perror()
#include <cstdlib>      // std::exit(), std::size_t
#include <iostream>     // std::cerr, std::endl
#include <span>         // std::span
#include <vector>       // std::vector

using Network::Buffer;
using Network::Fd;
using Network::Pathname;
using Network::fd_type;
using Network::os_error_type;
using Network::socket_error;
using Network::to_byte_string;

using ReadResult = std::pair<std::string, os_error_type>;

static bool verbose {false};  // NOLINT

static auto parse_arguments(int argc, char** argv) ->
    std::vector<std::string>
{
    std::vector<std::string> result {*argv};
    int ch {};

    while ((ch = ::getopt(argc, argv, "v")) != -1) {
        switch (ch) {
        case 'v':
            verbose = true;
            break;
        case '?':
            std::cerr << "Usage: "
                      << *argv
                      << " [-v]"
                      << std::endl;
            std::exit(EXIT_FAILURE);
        default:
            abort();
        }
    }

    const auto args = std::span(argv, std::size_t(argc));

    for (auto index = optind; index < argc; ++index) {
        result.emplace_back(args[index]);
    }

    return result;
}

static auto read(Fd fd) -> ReadResult
{
    Buffer buffer {BUFFER_SIZE};
    return {buffer, ::read(static_cast<fd_type>(fd),
                           buffer.data(),
                           buffer.size())};
}

static auto write(const std::string& str, Fd fd) -> os_error_type
{
    return ::write(static_cast<fd_type>(fd), str.c_str(), str.size() + 1);
}

auto main(int argc, char* argv[]) -> int
{
    bool shutdown {false};

    try {
        // Connect socket to socket address.
        const Fd fd {AF_UNIX, SOCK_SEQPACKET, 0, 0, verbose};
        const auto addr {to_byte_string(SOCKET_NAME)};
        const auto error {Network::connect(fd, addr, verbose)};
        const auto error_code {error.number()};

        if (error_code == socket_error) {
            std::cerr << error.string()
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // Fetch arguments from command line;
        const auto args = parse_arguments(argc, argv);

        // Send arguments to server.
        for (const auto& arg : args) {
            const auto code {write(arg, fd)};

            if (code == -1) {
                std::perror("write");
                break;
            }

            if (arg == "DOWN") {
                shutdown = true;
                break;
            }
        }

        if (!shutdown) {
            // Request result.
            const auto write_code {write("END", fd)};

            if (write_code == -1) {
                std::perror("write");
                std::exit(EXIT_FAILURE);
            }

            // Receive result.
            const auto [read_str, read_code] {read(fd)};

            if (read_code == -1) {
                std::perror("read");
                std::exit(EXIT_FAILURE);
            }

            std::cerr << "Result: "
                      << read_str
                      << std::endl;
        }

        // Close socket.
        Network::close(fd, verbose);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
