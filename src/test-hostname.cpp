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
#include "network/network.h"            // Context, Error, Hostname,
                                        // get_hostname(),
                                        // get_hostnameresult(),
                                        // start_context()
#include "network/parse.h"              // parse()

#ifdef WIN32
#include <winsock2.h>       // WSAEFAULT, WSAEPROCLIM,
                            // WSANOTINITIALISED, WSASYSNOTREADY,
                            // WSAVERNOTSUPPORTED
#endif

#include <cstddef>      // std::size_t
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <span>         // std::span
#include <string>       // std::string

namespace
{
    using Network::Error;
    using Network::Hostname;
    using Network::OsErrorResult;
    using Network::get_hostname;
    using Network::get_hostnameresult;
    using Network::parse;
    using Network::start_context;

#if defined(OS_CYGWIN_NT)
    constexpr auto expected_error_gethostname_re {""};
#elif defined(OS_MINGW64_NT)
    constexpr auto expected_error_gethostname_re {""};
#else
    constexpr auto expected_error_gethostname_re {
        R"(Call to ::gethostname\(.+\) failed with error \d+: .+)"
    };
#endif

    auto is_verbose {false};  // NOLINT

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
        std::cout << result.string()
                  << std::endl;
    }

    auto test_hostname_overflow() -> void
    {
        std::span<char> null {static_cast<char*>(nullptr), 0};
        std::string actual_str;

        if (auto result {get_hostnameresult(null, is_verbose)}) {
            print(result);
            actual_str = result.string();
        }

        const std::regex expected_regex {expected_error_gethostname_re};
        assert(std::regex_match(actual_str, expected_regex));
    }

    auto test_hostname_valid() -> void
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
        const auto context {start_context(is_verbose)};

        if (is_verbose) {
            std::cout << *context << std::endl;
        }

        test_hostname_overflow();
        test_hostname_valid();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
