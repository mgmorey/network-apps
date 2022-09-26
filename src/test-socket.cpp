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
#include "network/get-options.h"        // Arguments, get_options()
#include "network/network.h"            // Address, Context, FdPair,
                                        // OptionalPathname,
                                        // OsErrorResult, Pathname,
                                        // get_sun_path(),
                                        // string_null,
                                        // to_byte_string()
#include "network/to-sock-len.h"        // to_sock_len()

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
#include <span>         // std::span
#include <sstream>      // std::ostringstream
#include <string>       // std::string
#include <vector>       // std::vector

using Network::Address;
using Network::Arguments;
using Network::Context;
using Network::Error;
using Network::Fd;
using Network::FdPair;
using Network::LogicError;
using Network::OptionalPathname;
using Network::OsErrorResult;
using Network::Pathname;
using Network::RangeError;
using Network::get_options;
using Network::get_sockname;
using Network::get_sun_path;
using Network::get_sun_path_size;
using Network::os_error_type;
using Network::sock_len_max;
using Network::to_byte_string;
using Network::to_sock_len;

namespace TestSocket
{
    using ErrorCodeSet = std::set<os_error_type>;
    using OptionalPathnameVector = std::vector<OptionalPathname>;

    static constexpr auto expected_error_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of sock_len_type)"
    };
    static constexpr auto fd_width {6};
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

    static auto parse(const Arguments& args) -> void
    {
        auto options {get_options(args, "v")};

        if (options.find('?') != options.end()) {
            std::cerr << "Usage: "
                      << args[0]
                      << " [-v]"
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }

        if (options.find('v') != options.end()) {
            verbose = true;
        }
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
        const auto addr {to_byte_string(pathname)};
        assert(get_sun_path(addr) == pathname);

        if (pathname) {
            const Fd fd {AF_UNIX, SOCK_STREAM, 0, 0, true, verbose};
            const auto result {Network::bind(fd, addr, verbose)};
            actual_code = result.number();

            if (result) {
                print(result);
            }
            else {
                const auto self {get_sockname(fd.handle(), verbose)};
                std::cout << "Socket "
                          << std::right << std::setw(fd_width) << fd
                          << " bound to "
                          << Address(self)
                          << std::endl;
            }
        }
        else {
            assert(static_cast<bool>(pathname) == false);
        }

        assert(expected_codes.contains(actual_code));
    }

    static auto test_path_invalid(const OptionalPathname& pathname,
                                  const ErrorCodeSet& expected_codes) -> void
    {
        std::string actual_error_str;
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
            test_pathname(pathname, expected_codes);
        }
        catch (const LogicError& error) {
            actual_error_str = error.what();
            print(error);
        }

        assert(actual_error_str == expected_error);
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

    static auto test_socketpair() -> void
    {
        FdPair fds {AF_UNIX, SOCK_STREAM, 0, 0, verbose};
        std::cout << "Socket "
                  << std::right << std::setw(fd_width) << fds.at(0)
                  << " connected to "
                  << std::endl
                  << "Socket "
                  << std::right << std::setw(fd_width) << fds.at(1)
                  << std::endl;
    }

    static auto test_sock_len_invalid(long value) -> void
    {
        std::string actual_error_str;

        try {
            to_sock_len(value);
        }
        catch (const RangeError& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    static auto test_sock_len_invalid() -> void
    {
        test_sock_len_invalid(-1);
        test_sock_len_invalid(sock_len_max + 1);
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestSocket;

#ifndef OS_CYGWIN_NT
    static const ErrorCodeSet codes_no_directory {get_codes_no_directory()};
    static const ErrorCodeSet codes_no_permission {get_codes_no_permission()};
#endif
    static const ErrorCodeSet codes_valid = {0};

    try {
        const auto& context {Context::instance()};
        const Arguments args {argc, argv};
        parse(args);

        if (verbose) {
            std::cout << context;
        }

#ifndef OS_CYGWIN_NT
        test_path_no_permission("/foo", codes_no_permission);
        test_path_no_directory("/foo/bar", codes_no_directory);
#endif
        test_path_invalid(get_pathname(path_size_max), codes_valid);
        test_path_valid(get_pathname(path_size_max - 1), codes_valid);
        const auto& pathnames {get_pathnames()};

        for (const auto& pathname : pathnames) {
            test_path_valid(pathname, codes_valid);
        }

        test_socketpair();
        test_sock_len_invalid();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
