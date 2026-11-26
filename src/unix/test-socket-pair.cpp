// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/gtest.hpp"
#include "network/network.hpp"          // Address, ByteString, Error,
                                        // LogicError, OsError,
                                        // Pathname, Socket,
                                        // SocketHints, SocketPair,
                                        // UnixSocketHints, close(),
                                        // create_socketpair(),
                                        // handle_null, handle_type,
                                        // os_error_type,
                                        // path_length_max, run(),
                                        // to_bytestring(), to_path()
#include "network/parse.hpp"            // parse()

#include <sys/socket.h>     // AF_UNIX, AF_UNSPEC, SOCK_STREAM

#include <cstdlib>      // EXIT_FAILURE, std::exit(),
                        // std::size_t
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <set>          // std::set
#include <string>       // std::string

namespace
{
    using Network::ByteString;
    using Network::Error;
    using Network::Pathname;
    using Network::SharedRuntime;
    using Network::SocketHints;
    using Network::SocketPair;
    using Network::create_socketpair;
    using Network::family_type;
    using Network::handle_type;
    using Network::os_error_type;

    using ErrorCodeSet = std::set<os_error_type>;

    constexpr auto expected_socketpair_re {
        R"(Call to ::socketpair\(.+\) failed with error \d+: .+)",
    };
    constexpr auto handle_width {6};

    auto is_verbose {false};  // NOLINT

    auto print(const Error& error) -> void
    {
        if (is_verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    auto test_socketpair(const SocketHints& hints,
                         const std::string& expected_re) -> void
    {
        std::string actual_str;

        try {
            auto sp {create_socketpair(hints, is_verbose)};
            std::cout << "Socket "
                      << std::right << std::setw(handle_width) << *sp.at(0)
                      << " connected to "
                      << std::endl
                      << "Socket "
                      << std::right << std::setw(handle_width) << *sp.at(1)
                      << std::endl;
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        const std::regex expected_regex {expected_re};
        EXPECT_TRUE(std::regex_match(actual_str, expected_regex));
    }

    auto test_socketpair_invalid_protocol() -> void
    {
        const SocketHints hints {AF_UNIX, SOCK_STREAM, -1};
        test_socketpair(hints, expected_socketpair_re);
    }

    auto test_socketpair_invalid_socktype() -> void
    {
        const SocketHints hints {AF_UNIX, 0, 0};
        test_socketpair(hints, expected_socketpair_re);
    }

    auto test_socketpair_valid() -> void
    {
        const SocketHints hints {AF_UNIX, SOCK_STREAM, 0};
        test_socketpair(hints, "");
    }
}

// NOLINTNEXTLINE(bugprone-throwing-static-initialization)
TEST(SocketPair, InvalidAndValidHints)
{
    test_socketpair_invalid_socktype();
    test_socketpair_invalid_protocol();
    test_socketpair_valid();
}

#endif
