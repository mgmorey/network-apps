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

#include "network/arguments.h"          // Arguments
#include "network/assert.h"             // assert()
#include "network/get-option.h"         // get_optarg(), get_optind(),
                                        // get_option()

#include <algorithm>    // std::transform()
#include <cstdlib>      // EXIT_FAILURE, std::exit(), std::free()
#include <cstring>      // strdup()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <iterator>     // std::back_inserter()
#include <string>       // std::string
#include <vector>       // std::vector

using Network::Arguments;
using Network::get_optarg;
using Network::get_optind;
using Network::get_option;

namespace TestArguments
{
    using ArgumentVector = std::vector<Arguments::Argument>;
    using StringVector = std::vector<std::string>;

    static auto allocate_arguments(const StringVector& data) -> ArgumentVector
    {
        ArgumentVector args;
        std::transform(data.begin(), data.end(),
                       std::back_inserter(args),
                       [&](const std::string& datum) {
                           return ::strdup(datum.c_str());
                       });
        return args;
    }

    static auto free_arguments(const ArgumentVector& data) -> void
    {
        for (auto* datum : data) {
            ::free(datum);  // NOLINT
        }
    }

    static auto get_data(const char* argv0) -> StringVector
    {
        return {
            argv0,
            "-f",
            argv0,
            "-v",
            "one",
            "two",
            "three"
        };
    }

    static auto parse_arguments(std::string& filename, bool& verbose,
                                const Arguments& arguments) -> void
    {
#ifdef USING_GETOPT
        auto optind_begin {get_optind()};
#endif
        int opt {};

        while ((opt = get_option(arguments, "f:v")) != -1) {
            switch (opt) {
            case 'f':
                filename = get_optarg();
                break;
            case 'v':
                verbose = true;
                break;
            case '?':
                std::cerr << "Usage: "
                          << arguments[0]
                          << " [-v]"
                          << std::endl;
                std::exit(EXIT_FAILURE);
            default:
                abort();
            }

#ifdef USING_GETOPT
            if (opt != -1 && opt != '?') {
                assert(get_optind() > optind_begin);
                assert(get_optind() <= optind_begin + 2);
                optind_begin = get_optind();
            }
#endif
        }
    }

    static auto print_arguments(Arguments::ArgumentSpan args,
                                const std::string& scope = "All") -> void
    {
        auto index {0};

        for (const auto& arg : args) {
            std::cout << scope
                      << " argument "
                      << ++index
                      << ": "
                      << arg
                      << std::endl;
        }
    }

    static auto test_optional_arguments(Arguments::ArgumentSpan args,
                                        const char* argv0) -> void
    {
        print_arguments(args, "Optional");
        assert(std::string {args[0]} == "-f");
        assert(std::string {args[1]} == argv0);
        assert(std::string {args[2]} == "-v");
        assert(args.size() == 3);
    }

    static auto test_required_arguments(Arguments::ArgumentSpan args) -> void
    {
        print_arguments(args, "Required");
        assert(std::string {args[0]} == "one");
        assert(std::string {args[1]} == "two");
        assert(std::string {args[2]} == "three");
        assert(args.size() == 3);
    }

    static auto test_view_arguments(Arguments::ArgumentSpan args,
                                    const char* argv0) -> void
    {
        print_arguments(args, "View");
        assert(std::string {args[0]} == argv0);
        assert(std::string {args[1]} == "-f");
        assert(std::string {args[2]} == argv0);
        assert(std::string {args[3]} == "-v");
        assert(std::string {args[4]} == "one");
        assert(std::string {args[5]} == "two");
        assert(std::string {args[6]} == "three");
    }

    static auto test_arguments(int argc, char** argv) -> void
    {
        assert(argc > 0);
        assert(*argv != nullptr);
        auto data {allocate_arguments(get_data(*argv))};
        Arguments args {data.size(), data.data()};
        std::string filename;
        bool verbose {false};
        parse_arguments(filename, verbose, args);
        test_optional_arguments(args.optional(), *argv);
        test_required_arguments(args.required());
        test_view_arguments(args.view(), *argv);
        assert(filename == *argv);
        assert(verbose);
        free_arguments(data);
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestArguments;

    try {
        test_arguments(argc, argv);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
