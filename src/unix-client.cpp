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
#include "unix-common.h"                // BUFFER_SIZE, SOCKET_NAME

#include <sys/socket.h>         // SOCK_SEQPACKET
#include <sys/types.h>          // ssize_t
#include <sys/un.h>             // AF_UNIX
#include <unistd.h>             // ::read(), ::write()

#include <cerrno>       // EINVAL, EPROTONOSUPPORT
#include <cstdio>       // std::perror()
#include <cstdlib>      // std::exit(), std::size_t
#include <iostream>     // std::cerr, std::cout, std::endl
#include <span>         // std::span
#include <sstream>      // std::ostringstream
#include <vector>       // std::vector

using Network::Buffer;
using Network::CommandLine;
using Network::Fd;
using Network::Pathname;
using Network::connect;
using Network::fd_type;
using Network::format_os_error;
using Network::os_error_type;
using Network::to_byte_string;

using IoResult = std::pair<std::string, ssize_t>;

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

static auto parse_arguments(int argc, char** argv) ->
    CommandLine::ArgumentSpan
{
    CommandLine command_line(argc, argv, "v");
    int opt {};

    while ((opt = command_line.option()) != -1) {
        switch (opt) {
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

    return command_line.arguments(optind);
}

auto main(int argc, char* argv[]) -> int
{
    // Fetch arguments from command line;
    const auto args {parse_arguments(argc, argv)};

    try {
        bool shutdown {false};

        // Connect socket to socket address.
        const Fd fd {AF_UNIX, SOCK_SEQPACKET, 0, 0, false, verbose};
        const auto addr {to_byte_string(SOCKET_NAME)};
        const auto error {connect(fd, addr, verbose)};
        const auto error_code {error.number()};

        if (error_code != 0) {
            std::cerr << error.string()
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // Send arguments to server.
        for (const auto& arg : args) {
            const auto code {::write(fd_type {fd}, arg.data(), arg.size())};

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
            const auto write_code {::write(fd_type {fd}, "END", strlen("END"))};

            if (write_code == -1) {
                std::perror("write");
                std::exit(EXIT_FAILURE);
            }

            // Receive result.
            Buffer read_str {BUFFER_SIZE};
            const auto read_code {::read(fd_type {fd},
                                         read_str.data(),
                                         read_str.size())};

            if (read_code == -1) {
                std::perror("read");
                std::exit(EXIT_FAILURE);
            }

            std::cerr << "Result: "
                      << read_str
                      << std::endl;
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
