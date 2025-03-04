// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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
#include "network/commonsocket.h"       // CommonSocket
#include "network/network.h"            // Error, SocketData,
                                        // SocketHints, UniqueSocket,
                                        // handle_null, handle_type,
                                        // os_error_type,
                                        // start_context()
#include "network/parse.h"              // parse()

#ifdef WIN32
#include <winsock2.h>   // AF_INET, AF_UNSPEC, SOCK_STREAM
#else
#include <sys/socket.h> // AF_INET, AF_UNSPEC, SOCK_STREAM
#endif

#include <cerrno>       // EACCES, ENOENT, EROFS
#include <cstdlib>      // EXIT_FAILURE, std::exit(),
                        // std::size_t
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <memory>       // std::make_unique()
#include <regex>        // std::regex, std::regex_match
#include <string>       // std::string

namespace
{
    using Network::CommonSocket;
    using Network::Error;
    using Network::SocketData;
    using Network::SocketHints;
    using Network::UniqueSocket;
    using Network::create_socket;
    using Network::family_type;
    using Network::handle_null;
    using Network::handle_type;
    using Network::os_error_type;
    using Network::parse;
    using Network::start_context;

    class TestCommonSocket
        : public CommonSocket
    {
    public:
        explicit TestCommonSocket(const SocketData& t_data)
            : CommonSocket(t_data)
        {
        }

        auto family(family_type t_family) -> TestCommonSocket& final
        {
            static_cast<void>(CommonSocket::family(t_family));
            return *this;
        }

        auto handle(handle_type t_handle) -> TestCommonSocket& final
        {
            static_cast<void>(CommonSocket::handle(t_handle));
            return *this;
        }
    };

    constexpr auto expected_error_accept_re {
        R"(Call to ::accept\(.+\) failed with error \d+: .+)"
    };
    constexpr auto expected_error_handle_re {
        R"(Invalid socket descriptor value)"
    };
    constexpr auto expected_error_peername_re {
        R"(Call to ::getpeername\(.+\) failed with error \d+: .+)"
    };
#ifdef WIN32
    constexpr auto expected_error_read_re {
        R"(Call to ::recv\(.+\) failed with error \d+: .+)"
    };
#else
    constexpr auto expected_error_read_re {
        R"(Call to ::read\(.+\) failed with error \d+: .+)"
    };
#endif
    constexpr auto expected_error_socket_re {
        R"(Call to ::socket\(.+\) failed with error \d+: .+)"
    };
#ifdef WIN32
    constexpr auto expected_error_write_re {
        R"(Call to ::send\(.+\) failed with error \d+: .+)"
    };
#else
    constexpr auto expected_error_write_re {
        R"(Call to ::write\(.+\) failed with error \d+: .+)"
    };
#endif

    auto is_verbose {false}; // NOLINT

    auto create_null_socket() -> UniqueSocket
    {
        const SocketData sd {handle_null, AF_UNSPEC, true, true};
        return std::make_unique<TestCommonSocket>(sd);
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

    auto test_socket(handle_type handle,
                     const std::string& expected_error_re) -> void
    {
        std::string actual_error_str;

        try {
            const auto sock {create_socket(handle, AF_UNSPEC, is_verbose)};
            assert(static_cast<bool>(*sock));
            assert(static_cast<handle_type>(*sock) != handle_null);
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

    auto test_socket(const SocketHints& hints,
                     const std::string& expected_error_re) -> void
    {
        std::string actual_error_str;

        try {
            const auto sock {create_socket(hints, is_verbose)};
            assert(static_cast<bool>(*sock));
            assert(static_cast<handle_type>(*sock) != handle_null);
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

    auto test_socket_accept_invalid() -> void
    {
        const std::string expected_error_re {expected_error_accept_re};
        std::string actual_error_str;

        try {
            const auto sock {create_null_socket()};
            static_cast<void>(sock->accept());
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

    auto test_socket_close_invalid() -> void
    {
        const auto sock {create_null_socket()};
        static_cast<void>(Network::close(sock->handle()));
    }

    auto test_socket_family_invalid() -> void
    {
        const SocketHints hints {-1, SOCK_STREAM, 0};
        test_socket(hints, expected_error_socket_re);
    }

    auto test_socket_handle_invalid() -> void
    {
        test_socket(handle_null, expected_error_handle_re);
    }

    auto test_socket_listen_invalid() -> void
    {
        std::string actual_error_str;

        try {
            const auto sock {create_null_socket()};
            static_cast<void>(sock->listen(0));
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.empty());
    }

    auto test_socket_peername_invalid() -> void
    {
        const std::string expected_error_re {expected_error_peername_re};
        std::string actual_error_str;

        try {
            const auto sock {create_null_socket()};
            static_cast<void>(sock->peername());
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

    auto test_socket_protocol_invalid() -> void
    {
        const SocketHints hints {AF_INET, SOCK_STREAM, -1};
        test_socket(hints, expected_error_socket_re);
    }

    auto test_socket_read_invalid() -> void
    {
        const std::string expected_error_re {expected_error_read_re};
        std::string actual_error_str;

        try {
            const auto sock {create_null_socket()};
            static_cast<void>(sock->read(nullptr, 0));
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

    auto test_socket_shutdown_invalid() -> void
    {
        const auto sock {create_null_socket()};
        static_cast<void>(sock->shutdown(0));
    }

    auto test_socket_socktype_invalid() -> void
    {
        const SocketHints hints {AF_INET, -1, 0};
        test_socket(hints, expected_error_socket_re);
    }

    auto test_socket_write_invalid() -> void
    {
        const std::string expected_error_re {expected_error_write_re};
        std::string actual_error_str;

        try {
            const auto sock {create_null_socket()};
            static_cast<void>(sock->write({}));
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

    auto test_valid() -> void
    {
        const SocketHints hints {AF_INET, SOCK_STREAM, 0};
        test_socket(hints, "");
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

        test_socket_accept_invalid();
        test_socket_close_invalid();
        test_socket_family_invalid();
        test_socket_handle_invalid();
        test_socket_listen_invalid();
        test_socket_peername_invalid();
        test_socket_protocol_invalid();
        test_socket_read_invalid();
        test_socket_shutdown_invalid();
        test_socket_socktype_invalid();
        test_socket_write_invalid();
        test_valid();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
