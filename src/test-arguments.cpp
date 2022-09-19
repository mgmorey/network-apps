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

#include <cstdlib>      // EXIT_FAILURE, std::exit(), std::size_t
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl

using Network::Arguments;

namespace TestArguments
{
    static bool verbose {false};  // NOLINT

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

    static auto test_arguments(Arguments& arguments) -> void
    {
        const auto args {parse_arguments(arguments)};
        auto index {0};

        for (const auto& arg : args) {
            std::cout << "Argument "
                      << ++index
                      << ": "
                      << arg
                      << std::endl;
        }
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestArguments;

    try {
        Arguments arguments {argc, argv};
        test_arguments(arguments);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
