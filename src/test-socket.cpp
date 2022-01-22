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

#include "network/assert.h"             // assert()
#include "network/network.h"            // Address, Context, FdPair,
                                        // Hints, OptionalPathname,
                                        // Overloaded, OsErrorResult,
                                        // Socket, close(),
                                        // get_socketpair(),
                                        // get_sun_path(), is_valid(),
                                        // string_null,
                                        // to_byte_string()

#include <sys/socket.h>     // AF_UNIX, AF_UNSPEC, SOCK_STREAM
#include <sys/un.h>         // sockaddr_un
#include <unistd.h>         // getopt(), optarg, opterr, optind

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

    static auto test_path(const Network::OptionalPathname& pathname) -> void
    {
        const auto addr {Network::to_byte_string(pathname)};
        Network::OptionalPathname sun_path {Network::get_sun_path(addr)};

        if (pathname) {
            std::cout << "Unix domain path: "
                      << sun_path.value_or(Network::string_null)
                      << std::endl;
            assert(sun_path == pathname);
        }
        else {
            assert(static_cast<bool>(sun_path) == false);
        }

        assert(Network::is_valid(addr, verbose));
        Network::Address address {addr};
        std::cout << "Unix domain address: "
                  << address
                  << std::endl;
    }

    static auto test_socketpair(const Network::Hints& hints) -> void
    {
        const Network::Socket sock {hints};
        const auto socketpair_result {get_socketpair(sock, verbose)};
        std::visit(Network::Overloaded {
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
                [&](const Network::OsErrorResult& result) {
                    std::cerr << result.string()
                              << std::endl;
                }
            }, socketpair_result);
    }
}

auto main(int argc, char* argv[]) -> int
{
    static const Network::Socket hints {AF_UNIX, SOCK_STREAM};

    const std::vector<Network::OptionalPathname> paths = {
        std::nullopt,
        "/tmp/678",
        "/tmp/67890123456",
        "/tmp/6789012345678901234"
    };

    try {
        const auto args {TestSocket::parse_arguments(argc, argv)};
        const auto& context {Network::Context::instance()};

        if (TestSocket::verbose) {
            std::cerr << context;
        }

        TestSocket::test_socketpair(hints);

        for (const auto& path : paths) {
            TestSocket::test_path(path);
        }

        static_cast<void>(args);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
