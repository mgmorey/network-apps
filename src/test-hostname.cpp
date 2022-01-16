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

#include "network/network.h"            // OsErrorResult, Overloaded,
                                        // get_hostname()

#ifdef _WIN32
#include <getopt.h>         // getopt(), optarg, opterr, optind
#else
#include <unistd.h>         // getopt(), optarg, opterr, optind
#endif

#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <span>         // std::span
#include <variant>      // std::visit()
#include <vector>       // std::vector

namespace TestHostname
{
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

    static auto test_hostname() -> void
    {
        const auto hostname_result {Network::get_hostname()};
        std::visit(Network::Overloaded {
                [&](const std::string& hostname) {
                    std::cout << "Hostname: "
                              << hostname
                              << std::endl;
                },
                [&](const Network::OsErrorResult& result) {
                    std::cerr << "No hostname available: "
                              << result.string()
                              << std::endl;
                }
            }, hostname_result);
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        const auto args {TestHostname::parse_arguments(argc, argv)};
        const Network::Context context;

        if (TestHostname::verbose) {
            std::cerr << context;
        }

        TestHostname::test_hostname();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
