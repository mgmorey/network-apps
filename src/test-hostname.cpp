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
    constexpr auto expected_error_name_len_begin_max {
        "Value 1026 is out of range ["
    };
    constexpr auto expected_error_name_len_begin_min {
        "Value -1 is out of range ["
    };
    constexpr auto expected_error_name_len_end {
        "] of name_len_type"
    };

    static bool verbose {false};  // NOLINT

    static auto parse_arguments(int argc, char** argv) ->
        std::vector<std::string>
    {
        CommandLine command_line(argc, argv);
        int opt {};

        while ((opt = ::getopt(argc, argv, "v")) != -1) {
            switch (opt) {
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

        return command_line.arguments();
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

    static auto test_invalid_name_len() -> void
    {
        std::string actual_error_str;

        try {
            to_name_len(-1);
        }
        catch (const RangeError& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.starts_with(expected_error_name_len_begin_min));
        assert(actual_error_str.ends_with(expected_error_name_len_end));

        try {
            to_name_len(name_len_max + 1);
        }
        catch (const RangeError& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.starts_with(expected_error_name_len_begin_max));
        assert(actual_error_str.ends_with(expected_error_name_len_end));
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestHostname;

    try {
        parse_arguments(argc, argv);
        const auto& context {Context::instance()};

        if (verbose) {
            std::cout << context;
        }

        test_hostname();
        test_invalid_name_len();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
