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

#include "network/network.h"            // Context, OsErrorResult

#ifdef _WIN32
#include <getopt.h>         // getopt(), optarg, opterr, optind
#else
#include <unistd.h>         // getopt(), optarg, opterr, optind
#endif

#include <cassert>      // assert()
#include <climits>      // USHRT_MAX
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <span>         // std::span
#include <vector>       // std::vector

namespace TestContext
{
#ifdef _WIN32
    constexpr auto version_invalid {MAKEWORD(0, 0)};
#endif
    static bool verbose {false};  // NOLINT

    static auto parse_arguments(int argc, char** argv) ->
        std::vector<std::string>
    {
        std::vector<std::string> result {*argv};
        int ch {};

        while ((ch = ::getopt(argc, argv, "v")) != -1) {
            switch (ch) {
            case 'v':
                verbose = true;
                break;
            case '?':
                std::cerr << "Usage: "
                          << *argv
                          << " [-v]"
                          << std::endl;
                std::exit(EXIT_FAILURE);
            default:
                abort();
            }
        }

        const auto args = std::span(argv, size_t(argc));

        for (auto index = optind; index < argc; ++index) {
            result.emplace_back(args[index]);
        }

        return result;
    }

    static auto test_context(const Network::Context& context,
                             short unsigned version = USHRT_MAX) -> void
    {
        constexpr auto error_number {203};
        constexpr auto error_string {
            "Call to WSAStartup() returned 10092: The system could not "
            "find the environment option that was entered.\r"
        };
        const auto& result {context.result()};

        switch(version) {
#ifdef _WIN32
        case version_invalid:
            assert(result.number() == error_number);	// NOLINT
            assert(result.string() == error_string);	// NOLINT
            break;
#endif
        case USHRT_MAX:
            assert(result.number() == 0);		// NOLINT
            assert(result.string() == "");		// NOLINT
            break;
        default:
            assert(result.number() == 0);		// NOLINT
            assert(result.string() == "");		// NOLINT
        }
    }

    static auto test_context(short unsigned version) -> void
    {
        const Network::Context context {verbose, version};
        test_context(context, version);
    }

    static auto test_context() -> void
    {
        const Network::Context context {verbose};
        test_context(context);
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        const auto args {TestContext::parse_arguments(argc, argv)};
#ifdef _WIN32
        TestContext::test_context(TestContext::version_invalid);
#endif
        TestContext::test_context();
    }
    catch (std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
