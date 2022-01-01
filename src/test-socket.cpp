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

#include "network/network.h"        // Address, FdPair, Hints,
                                    // Overload, Socket, close(),
                                    // get_sockaddr(),
                                    // get_socketpair()

#include <sys/socket.h> // AF_UNIX, SOCK_STREAM
#include <sys/un.h>     // sockaddr_un
#include <unistd.h>     // getopt(), optarg, opterr, optind, optopt

#include <cassert>      // assert()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <optional>     // std::nullopt
#include <span>         // std::span
#include <string>       // std::string
#include <variant>      // std::visit()
#include <vector>       // std::vector

namespace TestSocket
{
    static constexpr auto PATH_12 {"/tmp/6789012"};
    static constexpr auto PATH_14 {"/tmp/678901234"};
    static constexpr auto PATH_16 {"/tmp/67890123456"};
    static constexpr auto PATH_20 {"/tmp/678901234567890"};

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

        const auto args = std::span(argv, size_t(argc));

        for (auto index = optind; index < argc; ++index) {
            result.emplace_back(args[index]);
        }

        return result;
    }

    static auto test_address_empty() -> void
    {
        const Network::Bytes addr;
        assert(Network::get_sa_family(addr, 0) == 0);  // NOLINT
        assert(Network::get_sa_length(addr, 0) == 0);  // NOLINT
    }

    static auto test_path(const Network::Pathname& pathname) -> void
    {
        Network::Address address {Network::get_sockaddr(pathname)};
        assert(is_valid(address, verbose));  // NOLINT
        std::cout << "Unix domain address: "
                  << address
                  << std::endl;
    }

    static auto test_socketpair(const Network::Hints& hints) -> void
    {
        const Network::Socket sock {hints};
        const auto socketpair_result {get_socketpair(sock, verbose)};
        std::visit(Network::Overload {
                [&](const Network::FdPair& fds) {
                    std::cout << "Socket "
                              << fds[0]
                              << " connected to socket "
                              << fds[1]
                              << std::endl;

                    Network::close(fds[0]);
                    Network::close(fds[1]);
                    std::cout << "Sockets "
                              << fds[0]
                              << " and "
                              << fds[1]
                              << " closed"
                              << std::endl;
                },
                [&](const Network::Result& result) {
                    std::cerr << result
                              << std::endl;
                }
            }, socketpair_result);
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        TestSocket::parse_arguments(argc, argv);
        TestSocket::test_address_empty();
        const Network::Socket hints(AF_UNIX, SOCK_STREAM);
        TestSocket::test_socketpair(hints);
        TestSocket::test_path(std::nullopt);
        TestSocket::test_path(TestSocket::PATH_12);
        TestSocket::test_path(TestSocket::PATH_14);
        TestSocket::test_path(TestSocket::PATH_16);
        TestSocket::test_path(TestSocket::PATH_20);
    }
    catch (std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
