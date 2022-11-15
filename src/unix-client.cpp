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
#include "network/network.h"            // Buffer, Socket, connect(),
                                        // socket_error,
                                        // to_bytestring()
#include "network/parse.h"              // parse()
#include "network/to-os-error.h"        // to_os_error()
#include "unix-common.h"                // BUFFER_SIZE, SOCKET_NAME

#include <sys/socket.h>         // SOCK_SEQPACKET
#include <sys/types.h>          // ssize_t
#include <sys/un.h>             // AF_UNIX
#include <unistd.h>             // ::read(), ::write()

#include <cerrno>       // EINVAL, EPROTONOSUPPORT
#include <cstdio>       // std::perror()
#include <cstdlib>      // std::exit(), std::size_t
#include <iostream>     // std::cerr, std::cout, std::endl
#include <sstream>      // std::ostringstream
#include <vector>       // std::vector

using Network::ArgumentSpan;
using Network::Buffer;
using Network::Pathname;
using Network::Socket;
using Network::connect;
using Network::descriptor_type;
using Network::format_os_error;
using Network::os_error_type;
using Network::parse;
using Network::to_bytestring;
using Network::to_os_error;

using IoResult = std::pair<std::string, ssize_t>;

static bool verbose {false};  // NOLINT

static auto format_message(int error) -> std::string
{
    const auto os_error {to_os_error(error)};
    std::ostringstream oss;
    oss << ("Call to ::socket(domain=AF_UNIX, "
            "type=SOCK_SEQPACKET, protocol=0) "
            "failed with error ")
        << error
        << ": "
        << format_os_error(os_error);
    return oss.str();
}

static auto parse(int argc, char** argv) -> ArgumentSpan
{
    const auto [operands, options] {parse(argc, argv, "v")};

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

    return operands;
}

static auto read(const Socket& fd) -> IoResult
{
    Buffer buffer {BUFFER_SIZE};
    return {buffer, ::read(descriptor_type {fd},
                           buffer.data(),
                           buffer.size())};
}

static auto write(const std::string& str, const Socket& fd) -> ssize_t
{
    return ::write(descriptor_type {fd}, str.data(), str.size());
}

auto main(int argc, char* argv[]) -> int
{
    const auto args {parse(argc, argv)};

    try {
        bool shutdown {false};

        // Connect socket to socket address.
        const Socket fd {AF_UNIX, SOCK_SEQPACKET, 0, 0, false, verbose};
        const auto addr {to_bytestring(SOCKET_NAME)};
        const auto error {connect(fd, addr, verbose)};
        const auto error_code {error.number()};

        if (error_code != 0) {
            std::cerr << error.string()
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // Send arguments to server.
        for (const auto& arg : args) {
            const std::string str {arg};
            const auto write_code = write(str, fd);

            if (write_code == -1) {
                std::perror("read");
                break;
            }

            if (str == "DOWN") {
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
    }
    catch (const std::exception& error) {
        if (error.what() != format_message(EINVAL) &&
            error.what() != format_message(EPROTONOSUPPORT)) {
            std::cerr << error.what()
                      << std::endl;
        }
    }
}
