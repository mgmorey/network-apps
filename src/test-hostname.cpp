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

#include "network/assert.hpp"           // assert()
#include "network/network.hpp"          // Error, Hostname, OsError,
                                        // SharedRuntime,
                                        // get_hostname(),
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
    using Network::SharedRuntime;
    using Network::get_hostname;
    using Network::get_hostnameresult;
    using Network::parse;
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

    auto test_get_hostnameresult_overflow(const SharedRuntime& sr) -> void
    {
        std::string hostname_str(1, '\0');
        std::string actual_str;

        if (auto error {get_hostnameresult(hostname_str, sr)}) {
            print(error);
            actual_str = error.string();
        }

        const std::regex expected_regex {expected_gethostname_re};
        assert(std::regex_match(actual_str, expected_regex));
    }

    auto test_get_hostnameresult_valid(const SharedRuntime& sr) -> void
    {
        std::string actual_str;
        const auto result {get_hostnameresult(sr)};

        if (!result) {
            print(result.error());
            actual_str = result.error().string();
        }

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
        const auto rt {run(is_verbose)};

        if (is_verbose) {
            std::cout << *rt << std::endl;
        }

        test_get_hostnameresult_overflow(rt);
        test_get_hostnameresult_valid(rt);
        test_get_hostname_valid();
    }
    catch (const Error& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
