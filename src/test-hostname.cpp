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
#include "network/network.h"            // Context, Error,
                                        // get_hostname()
#include "network/parse.h"              // parse()

#ifdef WIN32
#include <winsock2.h>       // WSAEFAULT, WSAEPROCLIM,
                            // WSANOTINITIALISED, WSASYSNOTREADY,
                            // WSAVERNOTSUPPORTED
#endif

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::string

namespace
{
    using Network::Error;
    using Network::Hostname;
    using Network::get_hostname;
    using Network::parse;
    using Network::start_context;

#if defined(OS_CYGWIN_NT)
    constexpr auto expected_error_length {
        ""
    };
#elif defined(OS_DARWIN)
    constexpr auto expected_error_length {
        ""
    };
#elif defined(OS_MINGW64_NT)
    constexpr auto expected_error_length {
        "Call to ::gethostname(, 1) failed with error 10014: "
        "The system detected an invalid pointer address in "
        "attempting to use a pointer argument in a call."
    };
#else
    constexpr auto expected_error_length {
        "Call to ::gethostname(, 1) failed with error 36: File name too long"
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

    auto test_hostname_length() -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(get_hostname(1, is_verbose));
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        if (*expected_error_length == '\0') {
            assert(actual_error_str.empty());
        }
        else {
            assert(actual_error_str == expected_error_length);
        }
    }

    auto test_hostname_valid() -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(get_hostname());
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.empty());
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

        test_hostname_length();
        test_hostname_valid();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
