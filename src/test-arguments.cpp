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

#include <cstdlib>      // EXIT_FAILURE, std::exit(), std::free()
#include <cstring>      // strdup()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <vector>       // std::vector

using Network::Arguments;

namespace TestArguments
{
    using ArgumentVector = std::vector<char*>;

    static bool verbose {false};  // NOLINT

    static auto allocate_arguments(char* argv0) -> ArgumentVector
    {
        ArgumentVector data;
        data.push_back(::strdup(argv0));
        data.push_back(::strdup("-v"));
        data.push_back(::strdup("foo"));
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
        int opt {};

        while ((opt = arguments.option("v")) != -1) {
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
        }

        return arguments.span();
    }

    static auto test_arguments(char* argv0) -> void
    {
        auto data {allocate_arguments(argv0)};
        Arguments arguments {data.size(), data.data()};
        const auto args {parse_arguments(arguments)};
        auto index {0};

        for (const auto& arg : args) {
            std::cout << "Argument "
                      << ++index
                      << ": "
                      << arg
                      << std::endl;
        }

        assert(verbose);
        assert(args.size() == 1);
        assert(std::string {args[0]} == "foo");
        free_arguments(data);
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestArguments;

    static_cast<void>(argc);
    try {
        test_arguments(*argv);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
