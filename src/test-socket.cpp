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

using Network::Address;
using Network::Context;
using Network::FdPair;
using Network::Hints;
using Network::OptionalPathname;
using Network::OsErrorResult;
using Network::Overloaded;
using Network::get_sun_path;
using Network::is_valid;
using Network::string_null;
using Network::to_byte_string;

namespace TestSocket
{
    using OptionalPathnameVector = std::vector<OptionalPathname>;

    static bool verbose {false};  // NOLINT

    auto get_pathname_vector() -> const OptionalPathnameVector&
    {
        static const OptionalPathnameVector pathname_vector {
            std::nullopt,
            "/tmp/678",
            "/tmp/67890123456",
            "/tmp/6789012345678901234"
        };
        return pathname_vector;
    }

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

    static auto test_socketpair(const Hints& hints) -> void
    {
        const auto socketpair_result {get_socketpair(hints, verbose)};
        std::visit(Overloaded {
                [&](const FdPair& fds) {
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
                [&](const OsErrorResult& result) {
                    std::cerr << result.string()
                              << std::endl;
                }
            }, socketpair_result);
    }

    static auto test_unix_path(const OptionalPathname& pathname) -> void
    {
        const auto addr {to_byte_string(pathname)};
        const auto unix_path {get_sun_path(addr)};

        if (pathname) {
            std::cout << "Unix domain path: "
                      << unix_path.value_or(string_null)
                      << std::endl;
            assert(unix_path == pathname);
        }
        else {
            assert(static_cast<bool>(unix_path) == false);
        }

        assert(is_valid(addr, verbose));
        Address address {addr};
        std::cout << "Unix domain address: "
                  << address
                  << std::endl;
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestSocket;

    static constexpr Hints hints {0, AF_UNIX, SOCK_STREAM};

    try {
        const auto args {parse_arguments(argc, argv)};
        const auto& context {Context::instance()};

        if (verbose) {
            std::cerr << context;
        }

        const auto& path_vector {get_pathname_vector()};

        for (const auto& path : path_vector) {
            test_unix_path(path);
        }

        test_socketpair(hints);
        static_cast<void>(args);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
