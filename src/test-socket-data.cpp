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

#include "network/assert.hpp"           // assert()
#include "network/network.hpp"          // Error, OsError, SocketCore,
                                        // close(), family_null,
                                        // family_type, handle_null,
                                        // handle_type, listen(),
                                        // os_error_type, run(),
                                        // shutdown()
#include "network/parse.hpp"            // parse()

#ifdef WIN32
#include <winsock2.h>   // AF_INET, AF_UNSPEC, SOCK_STREAM, ::close(),
                        // ::socket()
#else
#include <sys/socket.h> // AF_INET, AF_UNSPEC, SOCK_STREAM ::close(),
                        // ::socket()
#endif

#include <cerrno>       // EBADF
#include <cstdlib>      // EXIT_FAILURE, std::exit(),
                        // std::size_t
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <string>       // std::string

namespace
{
    using Network::Error;
    using Network::OsError;
    using Network::SharedRuntime;
    using Network::SocketCore;
    using Network::family_null;
    using Network::family_type;
    using Network::handle_null;
    using Network::handle_type;
    using Network::os_error_type;
    using Network::listen;
    using Network::parse;
    using Network::run;
    using Network::shutdown;

    constexpr auto handle_high {1000000000};
#ifdef WIN32
    constexpr auto expected_failed_listen_code {WSAENOTSOCK};
    constexpr auto expected_failed_shutdown_code {WSAENOTSOCK};
#else
    constexpr auto expected_failed_listen_code {EBADF};
    constexpr auto expected_failed_shutdown_code {EBADF};
#endif
    constexpr auto expected_runtime_pointer_re {
        R"(Null runtime pointer)"
    };
    constexpr auto expected_socket_descriptor_re {
        R"(Null socket descriptor)"
    };
    constexpr auto expected_socket_domain_family_re {
        R"(Null socket domain/family)"
    };
    constexpr auto expected_valid_re {
        R"()"
    };

    auto is_verbose {false}; // NOLINT

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

    auto print(const OsError& error) -> void
    {
        if (is_verbose) {
            std::cout << error.string()
                      << std::endl;
        }
    }

    auto test(handle_type handle,
              family_type family,
              const SharedRuntime& sr,
              const std::string& expected_re) -> void
    {
        std::string actual_str;

        try {
            const auto sc {SocketCore(handle, family, sr)};
            assert(sc.family() == family);
            assert(sc.handle() == handle);
            assert(sc.runtime() != nullptr);
            assert(sc.runtime()->is_verbose() == is_verbose);
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        if (expected_re.empty()) {
            assert(actual_str.empty());
        }
        else {
            const std::regex expected_regex {expected_re};
            assert(std::regex_match(actual_str, expected_regex));
        }
    }

    auto test_failed_listen(const SharedRuntime& sr) -> void
    {
        os_error_type actual_code {0};
        const SocketCore sc {handle_high, AF_INET, sr};

        if (const auto error {listen(sc, 0)}) {
            print(error);
            actual_code = error.number();
        }

        assert(actual_code == expected_failed_listen_code);
    }

    auto test_failed_shutdown(const SharedRuntime& sr) -> void
    {
        os_error_type actual_code {0};
        const SocketCore sc {handle_high, AF_INET, sr};

        if (const auto error {shutdown(sc, 0)}) {
            print(error);
            actual_code = error.number();
        }

        assert(actual_code == expected_failed_shutdown_code);
    }

    auto test_null_runtime_pointer() -> void
    {
        auto family {AF_INET};
        auto handle {::socket(family, SOCK_STREAM, 0)};
        test(handle, family, nullptr, expected_runtime_pointer_re);
    }

    auto test_null_socket_descriptor(const SharedRuntime& sr) -> void
    {
        auto family {AF_INET};
        test(handle_null, family, sr, expected_socket_descriptor_re);
    }

    auto test_null_socket_domain_family(const SharedRuntime& sr) -> void
    {
        auto handle {::socket(AF_INET, SOCK_STREAM, 0)};
        test(handle, family_null, sr, expected_socket_domain_family_re);
    }

    auto test_valid(const SharedRuntime& sr) -> void
    {
        auto family {AF_INET};
        auto handle {::socket(family, SOCK_STREAM, 0)};
        test(handle, family, sr, expected_valid_re);
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

        test_failed_listen(rt);
        test_failed_shutdown(rt);
        test_null_runtime_pointer();
        test_null_socket_descriptor(rt);
        test_null_socket_domain_family(rt);
        test_valid(rt);
    }
    catch (const Error& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
