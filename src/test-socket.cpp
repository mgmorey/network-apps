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
                                        // OptionalPathname,
                                        // Overloaded, OsErrorResult,
                                        // Socket, SocketHints,
                                        // close(), get_socketpair(),
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
#include <sstream>      // std::ostringstream
#include <string>       // std::string
#include <variant>      // std::visit()
#include <vector>       // std::vector

using Network::Address;
using Network::Context;
using Network::Error;
using Network::FdPair;
using Network::LogicError;
using Network::OptionalPathname;
using Network::OsErrorResult;
using Network::Overloaded;
using Network::Pathname;
using Network::SocketHints;
using Network::get_sun_path;
using Network::get_sun_path_size;
using Network::string_null;
using Network::to_byte_string;

namespace TestSocket
{
    using OptionalPathnameVector = std::vector<OptionalPathname>;

    static constexpr auto path_size_max {get_sun_path_size()};

    static bool verbose {false};  // NOLINT

    static auto get_pathname(std::size_t size) -> Pathname
    {
        const Pathname prefix {"/tmp/"};

        if (size > prefix.size()) {
            size -= prefix.size();
        }

        return prefix + Pathname(size, 'X');
    }

    static auto get_pathnames() -> OptionalPathnameVector
    {
        static constexpr auto size_max {64};
        OptionalPathnameVector pathnames;

        for (std::size_t size = size_max; size != 0; size /= 2) {
            pathnames.push_back(get_pathname(size));
        };

        pathnames.push_back(std::nullopt);
        return pathnames;
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

    static auto print(const Error& error) -> void
    {
        if (verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    static auto test_pathname(const OptionalPathname& pathname) -> void
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

        Address address {addr};
        std::cout << "Unix domain address: "
                  << address
                  << std::endl;
    }

    static auto test_path_invalid(const OptionalPathname& pathname) -> void
    {
        std::string actual_error;
        std::string expected_error;

        if (pathname) {
            std::ostringstream oss;
            oss << *pathname
                << ": pathname length of "
                << pathname->length()
                << " exceeds maximum of "
                << get_sun_path_size() - 1;
            expected_error = oss.str();
        }

        try {
            test_pathname(pathname);
        }
        catch (const LogicError& error) {
            print(error);
            actual_error = error.what();
        }

        assert(actual_error == expected_error);
    }

    static auto test_path_valid(const OptionalPathname& pathname) -> void
    {
        std::string actual_error;

        try {
            test_pathname(pathname);
        }
        catch (const LogicError& error) {
            print(error);
            actual_error = error.what();
        }

        assert(actual_error.empty());
    }

    static auto test_socketpair(const SocketHints& hints) -> void
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
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestSocket;

    static constexpr SocketHints hints {0, AF_UNIX, SOCK_STREAM};

    try {
        const auto args {parse_arguments(argc, argv)};
        const auto& context {Context::instance()};

        if (verbose) {
            std::cerr << context;
        }

        test_path_invalid(get_pathname(path_size_max));
        test_path_valid(get_pathname(path_size_max - 1));

        const auto& pathnames {get_pathnames()};

        for (const auto& pathname : pathnames) {
            test_path_valid(pathname);
        }

        test_socketpair(hints);
        static_cast<void>(args);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
