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
#include <exception>    // std::exception
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

    constexpr auto expected_error_socket_invalid_re {
        R"(Null socket descriptor)"
    };
    constexpr auto expected_error_socket_valid_re {
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
              const std::string& expected_error_re) -> void
    {
        std::string actual_error_str;

        try {
            const auto sd {SocketData(handle, family, is_verbose)};
            assert(sd.family() == family);
            assert(sd.handle() == handle);
            assert(sd.runtime()->is_verbose() == is_verbose);
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

    auto test_invalid() -> void
    {
        test(handle_null, family_null, expected_error_socket_invalid_re);
    }

    auto test_valid() -> void
    {
        auto family {AF_INET};
        auto handle {::socket(family, SOCK_STREAM, 0)};
        test(handle, family, expected_error_socket_valid_re);
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

        test_invalid();
        test_valid();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
