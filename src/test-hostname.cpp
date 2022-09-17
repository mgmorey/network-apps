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
#include "network/commandline.h"        // CommandLine
#include "network/network.h"            // Context, OsErrorResult,
                                        // get_hostname()
#include "network/to-name-len.h"        // to_name_len()

#ifdef WIN32
#include <getopt.h>         // getopt(), optarg, opterr, optind
#else
#include <unistd.h>         // getopt(), optarg, opterr, optind
#endif

#include <cstdlib>      // EXIT_FAILURE, std::exit(), std::size_t
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <span>         // std::span
#include <vector>       // std::vector

using Network::CommandLine;
using Network::Context;
using Network::Error;
using Network::Hostname;
using Network::RangeError;
using Network::get_hostname;
using Network::name_len_max;
using Network::to_name_len;

namespace TestHostname
{
    static constexpr auto expected_error_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of name_len_type)"
    };

    static bool verbose {false};  // NOLINT

    static auto parse_arguments(const CommandLine& command_line) -> void
    {
        int opt {};

        while ((opt = command_line.option("v")) != -1) {
            switch (opt) {
            case 'v':
                verbose = true;
                break;
            case '?':
                std::cerr << "Usage: "
                          << command_line[0]
                          << " [-v]"
                          << std::endl;
                std::exit(EXIT_FAILURE);
            default:
                abort();
            }
        }
    }

    static auto print(const Error& error) -> void
    {
        if (verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    static auto test_hostname() -> void
    {
        const auto hostname {get_hostname(verbose)};
        std::cout << "Hostname: "
                  << hostname
                  << std::endl;
    }

    static auto test_name_len_invalid(long value) -> void
    {
        std::string actual_error_str;

        try {
            to_name_len(value);
        }
        catch (const RangeError& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    static auto test_name_len_invalid() -> void
    {
        test_name_len_invalid(-1);
        test_name_len_invalid(name_len_max + 1);
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestHostname;

    try {
        const auto& context {Context::instance()};
        CommandLine command_line {argc, argv};
        parse_arguments(command_line);

        if (verbose) {
            std::cout << context;
        }

        test_hostname();
        test_name_len_invalid();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
