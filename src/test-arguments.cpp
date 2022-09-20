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
#include "network/get-option.h"         // get_optind(), get_option()

#include <cstdlib>      // EXIT_FAILURE, std::exit(), std::free()
#include <cstring>      // strdup()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <vector>       // std::vector

using Network::Arguments;
using Network::get_optind;
using Network::get_option;

namespace TestArguments
{
    using ArgumentVector = std::vector<char*>;

    static bool verbose {false};  // NOLINT

    static auto allocate_arguments(char* argv0) -> ArgumentVector
    {
        ArgumentVector data;
        data.push_back(::strdup(argv0));
        data.push_back(::strdup("-v"));
        data.push_back(::strdup("one"));
        data.push_back(::strdup("two"));
        data.push_back(::strdup("three"));
        return data;
    }

    static auto free_arguments(const ArgumentVector& data) -> void
    {
        for (auto* datum : data) {
            ::free(datum);  // NOLINT
        }
    }

    static auto parse_arguments(Arguments& arguments) ->
        Arguments::ArgumentSpan
    {
#ifdef USING_GETOPT
        auto optind_begin {get_optind()};
#endif
        int opt {};

        while ((opt = get_option(arguments, "v")) != -1) {
            switch (opt) {
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

        return arguments.span();
    }

    static auto test_arguments(Arguments::ArgumentSpan args) -> void
    {
        auto index {0};

        for (const auto& arg : args) {
            std::cout << "View argument "
                      << index++
                      << ": "
                      << arg
                      << std::endl;
        }

        assert(args.size() == 3);
        assert(std::string {args[0]} == "one");
        assert(std::string {args[1]} == "two");
        assert(std::string {args[2]} == "three");
    }

    static auto test_arguments(Arguments& args, const char* argv0) -> void
    {
        for (std::size_t index = 0; index < args.size(); ++index) {
            std::cout << "Command argument "
                      << index
                      << ": "
                      << args[index]
                      << std::endl;
        }

        assert(std::string {args[0]} == argv0);
        assert(std::string {args[1]} == "-v");
        assert(std::string {args[2]} == "one");
        assert(std::string {args[3]} == "two");
        assert(std::string {args[4]} == "three");
        test_arguments(parse_arguments(args));
        assert(verbose);
    }

    static auto test_arguments(int argc, char** argv) -> void
    {
        assert(argc > 0);
        assert(*argv != nullptr);
        auto data {allocate_arguments(*argv)};
        Arguments args {data.size(), data.data()};
        test_arguments(args, *argv);
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
