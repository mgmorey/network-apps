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

#include "network/always-false.hpp"     // always_false_v
#include "network/assert.hpp"           // assert()
#include "network/network.hpp"          // Error, Hostname,
                                        // get_hostnameresult(),
                                        // start_runtime()
#include "network/parse.hpp"            // parse()

#ifdef WIN32
#include <winsock2.h>       // WSAEFAULT, WSAEPROCLIM,
                            // WSANOTINITIALISED, WSASYSNOTREADY,
                            // WSAVERNOTSUPPORTED
#endif

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <string>       // std::string
#include <type_traits>  // std::decay_t, std::is_same_v
#include <variant>      // std::visit()

namespace
{
    using Network::Error;
    using Network::Hostname;
    using Network::OsErrorResult;
    using Network::always_false_v;
    using Network::get_hostname;
    using Network::get_hostnameresult;
    using Network::parse;
    using Network::start_runtime;

#if defined(OS_CYGWIN_NT)
    constexpr auto expected_error_gethostname_re {""};
#elif defined(OS_DARWIN)
    constexpr auto expected_error_gethostname_re {""};
#else
    constexpr auto expected_error_gethostname_re {
        R"(Call to ::gethostname\(.+\) failed with error \d+: .+)"
    };
#endif

    auto is_verbose {false};  // NOLINT

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

    auto print(const OsErrorResult& result) -> void
    {
        if (is_verbose) {
            std::cout << result.string()
                      << std::endl;
        }
    }

    auto test_get_hostnameresult_overflow() -> void
    {
        std::string hostname_str(1, '\0');
        std::string actual_str;

        if (auto result {get_hostnameresult(hostname_str,
                                            is_verbose)}) {
            print(result);
            actual_str = result.string();
        }

        const std::regex expected_regex {expected_error_gethostname_re};
        assert(std::regex_match(actual_str, expected_regex));
    }

    auto test_get_hostnameresult_valid() -> void
    {
        std::string actual_str;
        const auto hostname_result {get_hostnameresult(is_verbose)};
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, Hostname>) {
                static_cast<void>(arg);
            }
            else if constexpr (std::is_same_v<T, OsErrorResult>) {
                print(arg);
                actual_str = arg.string();
            }
            else {
                static_assert(always_false_v<T>, VISITOR_ERROR);
            }
        }, hostname_result);
        assert(actual_str.empty());
    }

    auto test_get_hostname_valid() -> void
    {
        std::string actual_str;

        try {
            static_cast<void>(get_hostname(is_verbose));
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }
        assert(actual_str.empty());
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        parse_arguments(argc, argv);
        const auto runtime {start_runtime(is_verbose)};

        if (is_verbose) {
            std::cout << *runtime << std::endl;
        }

        test_get_hostnameresult_overflow();
        test_get_hostnameresult_valid();
        test_get_hostname_valid();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
