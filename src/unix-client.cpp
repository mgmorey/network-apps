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
                                        // read_string() socket_error,
                                        // to_bytestring(), write()
#include "network/parse.h"              // parse()
#include "unix-common.h"                // BUFFER_SIZE, SOCKET_NAME

#include <sys/socket.h>         // SOCK_SEQPACKET
#include <sys/un.h>             // AF_UNIX

#include <cerrno>       // EINVAL, EPROTONOSUPPORT
#include <cstdio>       // std::perror()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <sstream>      // std::ostringstream
#include <vector>       // std::vector

using Network::ArgumentSpan;
using Network::Pathname;
using Network::Socket;
using Network::connect;
using Network::descriptor_type;
using Network::parse;
using Network::read_string;
using Network::to_bytestring;
using Network::write;

static bool verbose {false};  // NOLINT

static constexpr auto expected_error_socket_re {
    R"(Call to ::socket[(][^)]+[)] failed with error \d+: .+)"
};

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

auto main(int argc, char* argv[]) -> int
{
    const auto args {parse(argc, argv)};

    try {
        // Connect socket to socket address.
        const Socket sock {AF_UNIX, SOCK_SEQPACKET, 0, 0, false, verbose};
        const auto addr {to_bytestring(SOCKET_NAME)};
        const auto error {connect(sock, addr, verbose)};
        const auto error_code {error.number()};
        bool shutdown_pending {false};

        if (error_code != 0) {
            std::cerr << error.string() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // Send arguments to server.
        for (const auto& arg : args) {
            const std::string str {arg};
            const auto write_code = write(str, sock);

            if (write_code == -1) {
                std::perror("read");
                break;
            }

            if (str == "DOWN") {
                shutdown_pending = true;
                break;
            }
        }

        if (!shutdown_pending) {
            // Request result.
            const auto write_code {write("END", sock)};

            if (write_code == -1) {
                std::perror("write");
                std::exit(EXIT_FAILURE);
            }

            // Receive result.
            const auto [read_str, read_code] {read_string(BUFFER_SIZE, sock)};

            if (read_code == -1) {
                std::perror("read");
                std::exit(EXIT_FAILURE);
            }

            std::cerr << "Result: " << read_str << std::endl;
        }
    }
    catch (const std::exception& error) {
        const std::regex expected_error_regex {expected_error_socket_re};

        if (!std::regex_match(error.what(), expected_error_regex)) {
            std::cerr << error.what() << std::endl;
        }
    }
}
