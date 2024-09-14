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
#include "network/network.h"            // Address, ByteString, Error,
                                        // LogicError, OsErrorResult,
                                        // Pathname, Socket,
                                        // SocketHints, SocketPair,
                                        // UnixSocketHints, close(),
                                        // create_socket(),
                                        // create_socketpair(),
                                        // handle_null, handle_type,
                                        // os_error_type,
                                        // path_length_max,
                                        // start_context()
                                        // to_bytestring(), to_path()
#include "network/parse.h"              // parse()

#include <sys/socket.h>     // AF_UNIX, AF_UNSPEC, SOCK_STREAM

#include <cerrno>       // EACCES, EADDRINUSE, EBADF, EINVAL, ENOENT,
                        // EROFS
#include <cstdlib>      // EXIT_FAILURE, std::exit(),
                        // std::size_t
#include <exception>    // std::exception
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <set>          // std::set
#include <stack>        // std::stack
#include <string>       // std::string
#include <string_view>  // std::string_view

namespace Test
{
    using Network::Address;
    using Network::ByteString;
    using Network::Error;
    using Network::LogicError;
    using Network::OsErrorResult;
    using Network::Pathname;
    using Network::Socket;
    using Network::SocketHints;
    using Network::SocketPair;
    using Network::UnixSocketHints;
    using Network::close;
    using Network::create_socket;
    using Network::create_socketpair;
    using Network::handle_null;
    using Network::handle_type;
    using Network::os_error_type;
    using Network::parse;
    using Network::path_length_max;
    using Network::start_context;
    using Network::to_bytestring;
    using Network::to_path;

    using ErrorCodeSet = std::set<os_error_type>;

    static constexpr auto expected_error_path_length_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of path_length_type)"
    };
    static constexpr auto expected_error_payload_length_re {
        R"(Address payload length is zero: .+)"
    };
    static constexpr auto expected_error_socket_re {
        R"(Call to ::socket\(.+\) failed with error \d+: .+)"
    };
    static constexpr auto expected_error_socketpair_re {
        R"(Call to ::socketpair\(.+\) failed with error \d+: .+)"
    };
    static constexpr auto handle_width {6};
    static constexpr auto size_max {path_length_max};
    static constexpr auto size_min {6};  // NOLINT
    static constexpr UnixSocketHints socket_hints {SOCK_STREAM};

    static auto is_verbose {false};  // NOLINT

    auto operator==(const ByteString& addr,
                    const std::string_view& path) -> bool
    {
        return to_path(addr) == path;
    }

    auto get_pathname(std::string::size_type size) -> Pathname
    {
        const Pathname prefix {"/tmp/"};
        Pathname path(size, 'X');

        if (path.length() > prefix.length()) {
            path.replace(0, prefix.length(), prefix);
        }

        return path;
    }

    auto get_pathnames() -> std::stack<Pathname>
    {
        std::stack<Pathname> pathnames;

        for (std::size_t size = size_max; size >= size_min; size /= 2) {
            pathnames.push(get_pathname(size));
        };

        return pathnames;
    }

    auto parse_arguments(int argc, char** argv) -> void
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
            is_verbose = true;
        }

        static_cast<void>(_);
    }

    auto print(const Error& error) -> void
    {
        if (is_verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    auto print(const OsErrorResult& result) -> void
    {
        if (is_verbose) {
            std::cout << result.string()
                      << std::endl;
        }
    }

    auto print(const Socket& sock, const ByteString& addr) -> void
    {
        std::cout << "Socket "
                  << std::right << std::setw(handle_width) << sock
                  << " bound to "
                  << Address(addr)
                  << std::endl;
    }

    auto test_close(handle_type handle,
                    const ErrorCodeSet& expected_codes) -> void
    {
        os_error_type actual_code {};

        if (const auto result {close(handle, is_verbose)}) {
            print(result);
            actual_code = result.number();
        }

        assert(expected_codes.contains(actual_code));
    }

    auto test_close_handle_null() -> void
    {
	const ErrorCodeSet codes_bad_file_number {EBADF};
        test_close(handle_null, codes_bad_file_number);
    }

    auto test_path(const auto path,
                   Socket& sock,
                   const ErrorCodeSet& expected_codes,
                   const std::string& expected_error_re) -> void
    {
        std::string actual_error_str;

        try {
            assert(to_bytestring(path) == path);

            if (const auto result {sock.bind(path)}) {
                print(result);
                assert(expected_codes.contains(result.number()));
            }
            else {
                const auto addr {sock.sockname()};
                print(sock, addr);
                assert(addr == path);
            }
        }
        catch (const LogicError& error) {
            print(error);
            actual_error_str = error.what();
        }

        if (expected_error_re.empty()) {
            assert(actual_error_str.empty());
        }
        else {
            const std::regex expected_error_regex {expected_error_re};
            assert(std::regex_match(actual_error_str, expected_error_regex));
        }
    }

    auto test_path(const auto path,
                   const ErrorCodeSet& expected_codes,
                   const std::string& expected_error_re) -> void
    {
        const auto sock {create_socket(socket_hints, is_verbose)};
        return test_path(path, *sock, expected_codes, expected_error_re);
    }

    auto test_path_valid(const auto path) -> void
    {
        const auto sock {create_socket(socket_hints, is_verbose)};
        test_path(path, *sock, {0}, {});
    }

    auto test_paths_invalid() -> void
    {
#ifndef OS_CYGWIN_NT
	const ErrorCodeSet codes_invalid_directory {ENOENT};
#ifdef OS_DARWIN
	const ErrorCodeSet codes_invalid_permission {EACCES, EROFS};
#else
	const ErrorCodeSet codes_invalid_permission {EACCES};
#endif
#endif
	const ErrorCodeSet codes_valid {0};
        test_path("", codes_valid, expected_error_payload_length_re);
        const auto path_max {get_pathname(path_length_max + 1)};
        test_path(path_max, {}, expected_error_path_length_re);
#ifndef OS_CYGWIN_NT
        test_path("/foo/bar", codes_invalid_directory, {});
        test_path("/foo", codes_invalid_permission, {});
#endif
    }

    auto test_paths_valid() -> void
    {
        for (auto paths {get_pathnames()}; !paths.empty(); paths.pop()) {
            test_path_valid(paths.top());
        }
    }

    auto test_socketpair(const SocketHints& hints,
                         const std::string& expected_error_re) -> void
    {
        std::string actual_error_str;

        try {
            auto pair {create_socketpair(hints, is_verbose)};
            assert(static_cast<bool>(*pair[0]));
            assert(static_cast<bool>(*pair[1]));
            assert(static_cast<handle_type>(*pair[0]) != handle_null);
            assert(static_cast<handle_type>(*pair[1]) != handle_null);
            std::cout << "Socket "
                      << std::right << std::setw(handle_width) << *pair[0]
                      << " connected to "
                      << std::endl
                      << "Socket "
                      << std::right << std::setw(handle_width) << *pair[1]
                      << std::endl;
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        if (expected_error_re.empty()) {
            assert(actual_error_str.empty());
        }
        else {
            const std::regex expected_error_regex {expected_error_re};
            assert(std::regex_match(actual_error_str, expected_error_regex));
        }
    }

    auto test_socketpair_invalid_protocol() -> void
    {
        const SocketHints hints {AF_UNIX, SOCK_STREAM, -1};
        test_socketpair(hints, expected_error_socketpair_re);
    }

    auto test_socketpair_invalid_socktype() -> void
    {
        const SocketHints hints {AF_UNIX, 0, 0};
        test_socketpair(hints, expected_error_socketpair_re);
    }

    auto test_socketpair_valid() -> void
    {
        const SocketHints hints {AF_UNIX, SOCK_STREAM, 0};
        test_socketpair(hints, "");
    }

    auto test_unix_socket(const SocketHints& hints,
                          const std::string& expected_error_re) -> void
    {
        std::string actual_error_str;

        try {
            const auto sock {create_socket(hints, is_verbose)};
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        if (expected_error_re.empty()) {
            assert(actual_error_str.empty());
        }
        else {
            const std::regex expected_error_regex {expected_error_re};
            assert(std::regex_match(actual_error_str, expected_error_regex));
        }
    }

    auto test_unix_socket_invalid_protocol() -> void
    {
        const SocketHints hints {AF_UNIX, SOCK_STREAM, -1};
        test_unix_socket(hints, expected_error_socket_re);
    }

    auto test_unix_socket_invalid_socktype() -> void
    {
        const SocketHints hints {AF_UNIX, 0, 0};
        test_unix_socket(hints, expected_error_socket_re);
    }

    auto test_unix_socket_valid() -> void
    {
        const SocketHints hints {AF_UNIX, SOCK_STREAM, 0};
        test_unix_socket(hints, "");
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace Test;

    try {
        parse_arguments(argc, argv);
        const auto context {start_context(is_verbose)};

        if (is_verbose) {
            std::cout << *context << std::endl;
        }

        test_paths_invalid();
        test_socketpair_invalid_socktype();
        test_socketpair_invalid_protocol();
        test_unix_socket_invalid_socktype();
        test_unix_socket_invalid_protocol();
        test_close_handle_null();
        test_paths_valid();
        test_socketpair_valid();
        test_unix_socket_valid();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
