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
#include "network/network.hpp"          // Error, SocketData, close(),
                                        // family_null, family_type,
                                        // handle_null, handle_type,
                                        // os_error_type, run()
#include "network/parse.hpp"            // parse()

#ifdef WIN32
#include <winsock2.h>   // AF_INET, AF_UNSPEC, SOCK_STREAM, ::close(),
                        // ::socket()
#else
#include <sys/socket.h> // AF_INET, AF_UNSPEC, SOCK_STREAM ::close(),
                        // ::socket()
#endif

#include <cstdlib>      // EXIT_FAILURE, std::exit(),
                        // std::size_t
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <string>       // std::string

namespace
{
    using Network::Error;
    using Network::SharedRuntime;
    using Network::SocketData;
    using Network::family_null;
    using Network::family_type;
    using Network::handle_null;
    using Network::handle_type;
    using Network::os_error_type;
    using Network::parse;
    using Network::run;

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

    auto test(handle_type handle,
              family_type family,
              const SharedRuntime& sr,
              const std::string& expected_error_re) -> void
    {
        std::string actual_str;

        try {
            const auto sd {SocketData(handle, family, sr)};
            assert(sd.core().family() == family);
            assert(sd.core().handle() == handle);
            assert(sd.core().runtime() != nullptr);
            assert(sd.core().runtime()->is_verbose() == is_verbose);
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        if (expected_error_re.empty()) {
            assert(actual_str.empty());
        }
        else {
            const std::regex expected_error_regex {expected_error_re};
            assert(std::regex_match(actual_str, expected_error_regex));
        }
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
