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

#ifndef _WIN32

#include "network/assert.hpp"           // assert()
#include "network/network.hpp"          // Address, ByteString, Error,
                                        // LogicError, OsError,
                                        // Pathname, Socket,
                                        // SocketHints,
                                        // UnixSocketHints, close(),
                                        // create_socket(),
                                        // handle_null, handle_type,
                                        // os_error_type,
                                        // path_length_max, run(),
                                        // to_bytestring(), to_path()
#include "network/parse.hpp"            // parse()

#include <sys/socket.h>     // AF_UNIX, AF_UNSPEC, SOCK_STREAM

#include <cerrno>       // EACCES, EADDRINUSE, EBADF, EINVAL, ENOENT,
                        // EROFS
#include <cstdlib>      // EXIT_FAILURE, std::exit(),
                        // std::size_t
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <set>          // std::set
#include <stack>        // std::stack
#include <string>       // std::string
#include <string_view>  // std::string_view

namespace
{
    using Network::Address;
    using Network::ByteSpan;
    using Network::ByteString;
    using Network::Error;
    using Network::LogicError;
    using Network::OsError;
    using Network::Pathname;
    using Network::SharedRuntime;
    using Network::Socket;
    using Network::SocketHints;
    using Network::SocketPair;
    using Network::UniqueSocket;
    using Network::UnixSocketHints;
    using Network::create_socket;
    using Network::family_type;
    using Network::handle_type;
    using Network::os_error_type;
    using Network::parse;
    using Network::path_length_max;
    using Network::run;
    using Network::to_bytestring;
    using Network::to_path;

    using ErrorCodeSet = std::set<os_error_type>;

    constexpr auto expected_accept_re {
        R"(Call to ::accept\(.+\) failed with error \d+: .+)"
    };
    constexpr auto expected_path_length_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of path_length_type)"
    };
    constexpr auto expected_payload_length_re {
        R"(Address payload length is zero: .+)"
    };
    constexpr auto expected_socket_re {
        R"(Call to ::socket\(.+\) failed with error \d+: .+)"
    };
    constexpr auto handle_width {6};
    constexpr std::size_t size_max {path_length_max};
    constexpr std::size_t size_min {6};  // NOLINT
    constexpr UnixSocketHints socket_hints {SOCK_STREAM};

    auto is_verbose {false};  // NOLINT

    auto operator==(ByteSpan addr, std::string_view path) -> bool
    {
        const auto addr_path {to_path(addr)};
        return addr_path == path;
    }

    auto create_unix_socket() -> UniqueSocket
    {
        const SocketHints hints {AF_UNIX, SOCK_STREAM, 0};
        return create_socket(hints, is_verbose);
    }

    auto get_codes_no_such_file_or_directory() -> const ErrorCodeSet&
    {
        static const ErrorCodeSet codes {ENOENT};
        return codes;
    }

#ifndef __CYGWIN__

    auto get_codes_permission_denied() -> const ErrorCodeSet&
    {
#ifdef __APPLE__
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
        [[maybe_unused]] const auto [args, options] {parse(argc, argv, "v")};

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
    }

    auto print(const Error& error) -> void
    {
        if (is_verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    auto print(const OsError& result) -> void
    {
        if (is_verbose) {
            std::cout << result.string()
                      << std::endl;
        }
    }

    auto print(const Socket& sock, ByteSpan bs) -> void
    {
        std::cout << "Socket "
                  << std::right << std::setw(handle_width) << sock
                  << " bound to "
                  << Address(bs)
                  << std::endl;
    }

    auto test(Socket& sock,
              std::string_view path,
              const ErrorCodeSet& expected_codes,
              const std::string& expected_re) -> void
    {
        std::string actual_str;

        try {
            OsError actual_result;
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
            actual_str = error.what();
        }

        const std::regex expected_regex {expected_re};
        assert(std::regex_match(actual_str, expected_regex));
    }

    auto test(const SocketHints& hints,
              const std::string& expected_re) -> void
    {
        std::string actual_str;

        try {
            const auto sock {create_socket(hints, is_verbose)};
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        const std::regex expected_regex {expected_re};
        assert(std::regex_match(actual_str, expected_regex));
    }

    auto test(std::string_view path,
              const ErrorCodeSet& expected_codes,
              const std::string& expected_re) -> void
    {
        const auto sock {create_socket(socket_hints, is_verbose)};
        test(*sock, path, expected_codes, expected_re);
    }

    auto test_path_valid(const auto& path) -> void
    {
        const auto sock {create_socket(socket_hints, is_verbose)};
        test(*sock, path, {0}, {});
    }

    auto test_paths_invalid() -> void
    {
        test({}, {0}, expected_payload_length_re);
#if defined(__APPLE__) || defined(__CYGWIN__)
        test("", get_codes_no_such_file_or_directory(), {});
#endif
#ifndef __CYGWIN__
        test("/foo/bar", get_codes_no_such_file_or_directory(), {});
        test("/var/foo", get_codes_permission_denied(), {});
#endif
        const auto path_max {get_pathname(path_length_max + 1)};
        test(path_max, {}, expected_path_length_re);
    }

    auto test_paths_valid() -> void
    {
#if ! (defined(__APPLE__) || defined(__CYGWIN__))
        test("", {0}, {});
#endif

        for (auto paths {get_pathnames()}; !paths.empty(); paths.pop()) {
            test_path_valid(paths.top());
        }
    }

    auto test_unix_accept() -> void
    {
        std::string actual_str;

        try {
            const auto sock {create_unix_socket()};
            static_cast<void>(sock->accept());
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        const std::regex expected_regex {expected_accept_re};
        assert(std::regex_match(actual_str, expected_regex));
    }

    auto test_unix_invalid_protocol() -> void
    {
        const SocketHints hints {AF_UNIX, SOCK_STREAM, -1};
        test(hints, expected_socket_re);
    }

    auto test_unix_invalid_socktype() -> void
    {
        const SocketHints hints {AF_UNIX, 0, 0};
        test(hints, expected_socket_re);
    }

    auto test_unix_valid() -> void
    {
        const SocketHints hints {AF_UNIX, SOCK_STREAM, 0};
        test(hints, "");
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        parse_arguments(argc, argv);
        const auto rt {run(is_verbose)};

        if (is_verbose) {
            std::cout << *rt << std::endl;
        }

        test_paths_invalid();
        test_paths_valid();
        test_unix_invalid_socktype();
        test_unix_invalid_protocol();
        test_unix_accept();
        test_unix_valid();
    }
    catch (const Error& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}

#endif
