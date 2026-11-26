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

#include "network/gtest.hpp"
#include "network/network.hpp"          // Error, Hostname, OsError,
                                        // Runtime, get_hostname(),
                                        // get_hostnameresult(), run()
#include "network/parse.hpp"            // parse()

#ifdef _WIN32
#include <winsock2.h>       // WSAEFAULT, WSAEPROCLIM,
                            // WSANOTINITIALISED, WSASYSNOTREADY,
                            // WSAVERNOTSUPPORTED
#endif

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <string>       // std::string

namespace
{
    using Network::Error;
    using Network::Hostname;
    using Network::OsError;
    using Network::Runtime;
    using Network::get_hostname;
    using Network::get_hostnameresult;
    using Network::run;

#if defined(__APPLE__) && defined(__MACH__)
    constexpr auto expected_gethostname_re {
        R"()"
    };
#elif defined(__CYGWIN__) && !defined(_WIN32)
    constexpr auto expected_gethostname_re {
        R"()"
    };
#else
    constexpr auto expected_gethostname_re {
        R"(Call to ::gethostname\(.+\) failed with error \d+: .+)"
    };
#endif

    auto is_verbose {false};  // NOLINT

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

    auto test_get_hostnameresult_overflow(const Runtime* rt) -> void
    {
        std::string actual_str;
        std::string hostname_str(1, '\0');

        if (const auto error {get_hostname(hostname_str, rt)}) {
            print(error);
            actual_str = error.string();
        }

        const std::regex expected_regex {expected_gethostname_re};
        EXPECT_TRUE(std::regex_match(actual_str, expected_regex));
    }

    auto test_get_hostnameresult_valid(const Runtime* rt) -> void
    {
        std::string actual_str;
        const auto result {get_hostnameresult(rt)};

        if (!result) {
            print(result.error());
            actual_str = result.error().string();
        }

        EXPECT_TRUE(actual_str.empty());
    }

    auto test_get_hostname_valid() -> void
    {
        std::string actual_str;

        try {
            const auto hostname {get_hostname(is_verbose)};
            std::cout << hostname << std::endl;
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        EXPECT_TRUE(actual_str.empty());
    }
}

// NOLINTNEXTLINE(bugprone-throwing-static-initialization)
TEST(Hostname, ResultAndLocalHostname)
{
    const auto sr {run(false)};
    const auto* rt {sr.get()};
    test_get_hostnameresult_overflow(rt);
    test_get_hostnameresult_valid(rt);
    test_get_hostname_valid();
}
