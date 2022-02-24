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
                                        // get_socketpair(),
                                        // get_sun_path(),
                                        // string_null,
                                        // to_byte_string()

#include <sys/socket.h>     // AF_UNIX, AF_UNSPEC, SOCK_STREAM
#include <sys/un.h>         // sockaddr_un
#include <unistd.h>         // getopt(), optarg, opterr, optind

#include <cerrno>       // EACCES, ENOENT, EROFS
#include <cstdio>       // std::remove()
#include <cstdlib>      // EXIT_FAILURE, std::exit(), std::size_t
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <optional>     // std::nullopt
#include <set>          // std::set
#include <span>         // std::span
#include <sstream>      // std::ostringstream
#include <string>       // std::string
#include <variant>      // std::visit()
#include <vector>       // std::vector

using Network::Address;
using Network::Context;
using Network::Error;
using Network::Fd;
using Network::FdPair;
using Network::FdResult;
using Network::LogicError;
using Network::OptionalPathname;
using Network::OsErrorResult;
using Network::Overloaded;
using Network::Pathname;
using Network::SocketHints;
using Network::get_socket;
using Network::get_sun_path;
using Network::get_sun_path_size;
using Network::os_error_type;
using Network::string_null;
using Network::to_byte_string;

namespace TestSocket
{
    using ErrorCodeSet = std::set<os_error_type>;
    using OptionalPathnameVector = std::vector<OptionalPathname>;

    static constexpr auto path_size_max {get_sun_path_size()};

    static bool verbose {false};  // NOLINT

#ifndef OS_CYGWIN_NT

    static auto get_codes_no_directory() -> const ErrorCodeSet&
    {
        static const ErrorCodeSet codes = {ENOENT};
        return codes;
    }

    static auto get_codes_no_permission() -> const ErrorCodeSet&
    {
#ifdef OS_DARWIN
        static const ErrorCodeSet codes = {EACCES, EROFS};
#else
        static const ErrorCodeSet codes = {EACCES};
#endif
        return codes;
    }

#endif

    static auto get_pathname(std::string::size_type size) -> Pathname
    {
        const Pathname prefix("/tmp/");
        Pathname pathname(size, 'X');

        if (pathname.length() > prefix.length()) {
            pathname.replace(0, prefix.length(), prefix);
        }

        return pathname;
    }

    static auto get_pathnames() -> OptionalPathnameVector
    {
        static constexpr auto size_max {64};
        static constexpr auto size_min {8};
        OptionalPathnameVector pathnames;

        for (std::size_t size = size_max; size >= size_min; size /= 2) {
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

        const auto args = std::span(argv, std::size_t(argc));

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

    static auto print(const OsErrorResult& result) -> void
    {
        if (verbose) {
            std::cout << "Number: "
                      << result.number()
                      << std::endl
                      << "String: "
                      << result.string()
                      << std::endl;
        }
    }

    static auto test_pathname(const SocketHints& hints,
                              const OptionalPathname& pathname,
                              const ErrorCodeSet& expected_codes) -> void
    {
        os_error_type actual_code {0};
        const auto addr {to_byte_string(pathname)};
        std::cout << "Unix domain address: "
                  << Address(addr)
                  << std::endl;
        const auto unix_path {get_sun_path(addr)};
        std::cout << "Unix domain path: "
                  << unix_path.value_or(string_null)
                  << std::endl;

        if (pathname) {
            assert(unix_path == pathname);
            const auto socket_result {get_socket(hints, verbose)};
            std::visit(Overloaded {
                    [&](const Fd& fd) {
                        const auto result {Network::bind(fd, addr, verbose)};
                        actual_code = result.number();

                        if (result) {
                            print(result);
                        }
                        else if (unix_path) {
                            std::remove(unix_path->c_str());
                        }
                    },
                    [&](const OsErrorResult& result) {
                        print(result);
                    }
                }, socket_result);
        }
        else {
            assert(static_cast<bool>(unix_path) == false);
        }

        assert(expected_codes.count(actual_code) != 0);
    }

    static auto test_path_invalid(const SocketHints& hints,
                                  const OptionalPathname& pathname,
                                  const ErrorCodeSet& expected_codes) -> void
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
            test_pathname(hints, pathname, expected_codes);
        }
        catch (const LogicError& error) {
            actual_error = error.what();
            print(error);
        }

        assert(actual_error == expected_error);
    }

#ifndef OS_CYGWIN_NT

    static auto test_path_no_directory(const SocketHints& hints,
                                       const Pathname& pathname,
                                       const ErrorCodeSet& expected_codes) -> void
    {
        std::string actual_error;

        try {
            test_pathname(hints, pathname, expected_codes);
        }
        catch (const LogicError& error) {
            print(error);
            actual_error = error.what();
        }

        assert(actual_error.empty());
    }

    static auto test_path_no_permission(const SocketHints& hints,
                                        const Pathname& pathname,
                                        const ErrorCodeSet& expected_codes) -> void
    {
        std::string actual_error;

        try {
            test_pathname(hints, pathname, expected_codes);
        }
        catch (const LogicError& error) {
            print(error);
            actual_error = error.what();
        }

        assert(actual_error.empty());
    }

#endif

    static auto test_path_valid(const SocketHints& hints,
                                const OptionalPathname& pathname,
                                const ErrorCodeSet& expected_codes) -> void
    {
        std::string actual_error;

        try {
            test_pathname(hints, pathname, expected_codes);
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
#ifndef OS_CYGWIN_NT
    static const ErrorCodeSet codes_no_directory {get_codes_no_directory()};
    static const ErrorCodeSet codes_no_permission {get_codes_no_permission()};
#endif
    static const ErrorCodeSet codes_valid = {0};

    try {
        parse_arguments(argc, argv);
        const auto& context {Context::instance()};

        if (verbose) {
            std::cerr << context;
        }

#ifndef OS_CYGWIN_NT
        test_path_no_permission(hints, "/foo", codes_no_permission);
        test_path_no_directory(hints, "/foo/bar", codes_no_directory);
#endif
        test_path_invalid(hints, get_pathname(path_size_max), codes_valid);
        test_path_valid(hints, get_pathname(path_size_max - 1), codes_valid);
        const auto& pathnames {get_pathnames()};

        for (const auto& pathname : pathnames) {
            test_path_valid(hints, pathname, codes_valid);
        }

        test_socketpair(hints);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
