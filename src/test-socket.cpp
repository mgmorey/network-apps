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
#include "network/network.h"            // Address, Context, Error,
                                        // LogicError
                                        // OptionalPathname,
                                        // OsErrorResult, Pathname,
                                        // Socket, SocketPair, bind(),
                                        // get_sun_path(),
                                        // os_error_type,
                                        // to_bytestring()
#include "network/parse.h"              // parse()

#include <sys/socket.h>     // AF_UNIX, AF_UNSPEC, SOCK_STREAM
#include <sys/un.h>         // sockaddr_un

#include <cerrno>       // EACCES, ENOENT, EROFS
#include <cstdlib>      // EXIT_FAILURE, std::exit(),
                        // std::size_t
#include <exception>    // std::exception
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <optional>     // std::nullopt
#include <regex>        // std::regex, std::regex_match
#include <set>          // std::set
#include <sstream>      // std::ostringstream
#include <string>       // std::string
#include <vector>       // std::vector

namespace TestSocket
{
    using Network::Address;
    using Network::Context;
    using Network::Error;
    using Network::LogicError;
    using Network::OptionalPathname;
    using Network::OsErrorResult;
    using Network::Pathname;
    using Network::RangeError;
    using Network::Socket;
    using Network::SocketPair;
    using Network::bind;
    using Network::to_path;
    using Network::os_error_type;
    using Network::parse;
    using Network::path_len_max;
    using Network::to_bytestring;

    using ErrorCodeSet = std::set<os_error_type>;
    using OptionalPathnameVector = std::vector<OptionalPathname>;

    static constexpr auto expected_error_path_len_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of path_len_type)"
    };
    static constexpr auto handle_width {6};

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
        const Pathname prefix {"/tmp/"};
        Pathname pathname(size, 'X');

        if (pathname.length() > prefix.length()) {
            pathname.replace(0, prefix.length(), prefix);
        }

        return pathname;
    }

    static auto get_pathnames() -> OptionalPathnameVector
    {
        static constexpr auto size_max {64};	// NOLINT
        static constexpr auto size_min {8};	// NOLINT

        OptionalPathnameVector pathnames {{}};

        for (std::size_t size = size_min; size <= size_max; size *= 2) {
            pathnames.push_back(get_pathname(size));
        };

        pathnames.push_back(std::nullopt);
        return pathnames;
    }

    static auto parse(int argc, char** argv) -> void
    {
        const auto [_, options] {parse(argc, argv, "v")};

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

        static_cast<void>(_);
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

    static auto test_pathname(const OptionalPathname& pathname,
                              const ErrorCodeSet& expected_codes) -> void
    {
        os_error_type actual_code {0};
        const auto addr {to_bytestring(pathname)};
        assert(to_path(addr) == pathname);

        if (pathname) {
            const Socket sock {AF_UNIX, SOCK_STREAM, 0, 0, true, verbose};
            const auto result {bind(sock, addr, verbose)};
            actual_code = result.number();

            if (result) {
                print(result);
            }
            else {
                const auto self {sock.sockname()};
                std::cout << "Socket "
                          << std::right << std::setw(handle_width) << sock
                          << " bound to "
                          << Address(self)
                          << std::endl;
            }
        }

        assert(expected_codes.contains(actual_code));
    }

    static auto test_path_invalid(const OptionalPathname& pathname,
                                  const ErrorCodeSet& expected_codes) -> void
    {
        std::string actual_error_str;

        try {
            test_pathname(pathname, expected_codes);
        }
        catch (const RangeError& error) {
            actual_error_str = error.what();
            print(error);
        }

        const std::regex expected_error_regex {expected_error_path_len_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

#ifndef OS_CYGWIN_NT

    static auto test_path_no_directory(const Pathname& pathname,
                                       const ErrorCodeSet& expected_codes) -> void
    {
        std::string actual_error_str;

        try {
            test_pathname(pathname, expected_codes);
        }
        catch (const LogicError& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.empty());
    }

    static auto test_path_no_permission(const Pathname& pathname,
                                        const ErrorCodeSet& expected_codes) -> void
    {
        std::string actual_error_str;

        try {
            test_pathname(pathname, expected_codes);
        }
        catch (const LogicError& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.empty());
    }

#endif

    static auto test_path_valid(const OptionalPathname& pathname,
                                const ErrorCodeSet& expected_codes) -> void
    {
        std::string actual_error_str;

        try {
            test_pathname(pathname, expected_codes);
        }
        catch (const LogicError& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.empty());
    }

    static auto test_paths() -> void
    {
#ifndef OS_CYGWIN_NT
        static const ErrorCodeSet codes_no_directory {get_codes_no_directory()};
        static const ErrorCodeSet codes_no_permission {get_codes_no_permission()};
#endif
        static const ErrorCodeSet codes_valid = {0};

        const auto& pathnames {get_pathnames()};

        for (const auto& pathname : pathnames) {
            test_path_valid(pathname, codes_valid);
        }

        test_path_valid(get_pathname(path_len_max - 1), codes_valid);
        test_path_invalid(get_pathname(path_len_max), codes_valid);
#ifndef OS_CYGWIN_NT
        test_path_no_directory("/foo/bar", codes_no_directory);
        test_path_no_permission("/foo", codes_no_permission);
#endif
    }

    static auto test_socketpair() -> void
    {
        SocketPair pair {AF_UNIX, SOCK_STREAM, 0, 0, verbose};
        std::cout << "Socket "
                  << std::right << std::setw(handle_width) << pair.at(0)
                  << " connected to "
                  << std::endl
                  << "Socket "
                  << std::right << std::setw(handle_width) << pair.at(1)
                  << std::endl;
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestSocket;

    try {
        const auto& context {Context::instance()};
        parse(argc, argv);

        if (verbose) {
            std::cout << context;
        }

        test_socketpair();
        test_paths();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
