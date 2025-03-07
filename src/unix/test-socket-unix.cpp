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

#ifndef WIN32

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
#include <cstddef>      // std::byte
#include <cstdlib>      // EXIT_FAILURE, std::exit(),
                        // std::size_t
#include <exception>    // std::exception
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <set>          // std::set
#include <span>         // std::span
#include <stack>        // std::stack
#include <string>       // std::string
#include <string_view>  // std::string_view

namespace
{
    using Network::Address;
    using Network::ByteString;
    using Network::Error;
    using Network::LogicError;
    using Network::OsErrorResult;
    using Network::Pathname;
    using Network::Socket;
    using Network::SocketData;
    using Network::SocketHints;
    using Network::SocketPair;
    using Network::UnixSocketHints;
    using Network::close;
    using Network::create_socket;
    using Network::create_socketpair;
    using Network::family_type;
    using Network::handle_null;
    using Network::handle_type;
    using Network::os_error_type;
    using Network::parse;
    using Network::path_length_max;
    using Network::start_context;
    using Network::to_bytestring;
    using Network::to_path;

    using ErrorCodeSet = std::set<os_error_type>;

    constexpr auto expected_error_path_length_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of path_length_type)"
    };
    constexpr auto expected_error_payload_length_re {
        R"(Address payload length is zero: .+)"
    };
    constexpr auto expected_error_socket_re {
        R"(Call to ::socket\(.+\) failed with error \d+: .+)"
    };
    constexpr auto expected_error_socketpair_re {
        R"(Call to ::socketpair\(.+\) failed with error \d+: .+)"
    };
    constexpr auto handle_width {6};
    constexpr std::size_t size_max {path_length_max};
    constexpr std::size_t size_min {6};  // NOLINT
    constexpr UnixSocketHints socket_hints {SOCK_STREAM};

    auto is_verbose {false};  // NOLINT

    auto operator==(std::span<const std::byte> addr,
                    std::string_view path) -> bool
    {
        const auto addr_path {to_path(addr)};
        return addr_path == path;
    }

    auto operator==(std::span<const std::byte> addr,
                    const std::nullptr_t& path) -> bool
    {
        const auto addr_path {to_path(addr)};
        static_cast<void>(path);
        return addr_path.empty();
    }

    auto get_codes_bad_file_number() -> const ErrorCodeSet&
    {
	static const ErrorCodeSet codes {EBADF};
        return codes;
    }

    auto get_codes_no_such_file_or_directory() -> const ErrorCodeSet&
    {
        static const ErrorCodeSet codes {ENOENT};
        return codes;
    }

#ifndef OS_CYGWIN_NT

    auto get_codes_permission_denied() -> const ErrorCodeSet&
    {
#ifdef OS_DARWIN
        static const ErrorCodeSet codes {EACCES, EROFS};
#else
	static const ErrorCodeSet codes {EACCES};
#endif
        return codes;
    }

#endif

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

    auto print(const Socket& sock, std::span<const std::byte> bs) -> void
    {
        std::cout << "Socket "
                  << std::right << std::setw(handle_width) << sock
                  << " bound to "
                  << Address(bs)
                  << std::endl;
    }

    auto test_close(handle_type handle,
                    family_type family,
                    const ErrorCodeSet& expected_codes) -> void
    {
        os_error_type actual_code {};

        const SocketData sd {handle, family, is_verbose};
        if (const auto result {close(sd)}) {
            print(result);
            actual_code = result.number();
        }

        assert(expected_codes.contains(actual_code));
    }

    auto test_close_handle_null() -> void
    {
        test_close(handle_null, AF_UNIX, get_codes_bad_file_number());
    }

    auto test_path(Socket& sock,
                   const auto& path,
                   const ErrorCodeSet& expected_codes,
                   const std::string& expected_error_re) -> void
    {
        std::string actual_error_str;

        try {
            OsErrorResult actual_result;
            const auto bs {to_bytestring(path)};  // NOLINT
            assert(bs == path);  // NOLINT

            if (const auto result {sock.bind(bs)}) {
                print(result);
                actual_result = result;
            }
            else {
                const auto self {sock.sockname()};
                print(sock, self);
                assert(self == path);  // NOLINT
            }

            assert(expected_codes.contains(actual_result.number()));
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

    auto test_path(const auto& path,
                   const ErrorCodeSet& expected_codes,
                   const std::string& expected_error_re) -> void
    {
        const auto sock {create_socket(socket_hints, is_verbose)};
        return test_path(*sock, path, expected_codes, expected_error_re);
    }

    auto test_path_valid(const auto& path) -> void
    {
        const auto sock {create_socket(socket_hints, is_verbose)};
        test_path(*sock, path, {0}, {});
    }

    auto test_paths_invalid() -> void
    {
        test_path(nullptr, {0}, expected_error_payload_length_re);
#if defined(OS_DARWIN) || defined(OS_CYGWIN_NT)
        test_path("", get_codes_no_such_file_or_directory(), {});
#endif
#ifndef OS_CYGWIN_NT
        test_path("/foo/bar", get_codes_no_such_file_or_directory(), {});
        test_path("/var/foo", get_codes_permission_denied(), {});
#endif
        const auto path_max {get_pathname(path_length_max + 1)};
        test_path(path_max, {}, expected_error_path_length_re);
    }

    auto test_paths_valid() -> void
    {
#if ! (defined(OS_DARWIN) || defined(OS_CYGWIN_NT))
        test_path("", {0}, {});
#endif

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

#endif
