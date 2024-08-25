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
#include "network/network.h"            // Context, Hostname,
                                        // get_hostname()
#include "network/parse.h"              // parse()

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl

namespace Test
{
    using Network::Context;
    using Network::Hostname;
    using Network::get_hostname;
    using Network::parse;

    static auto is_verbose {false};  // NOLINT

    auto parse_arguments(int argc, char** argv) -> void
    {
        const auto [_, options] {parse(argc, argv, "v")};

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

        static_cast<void>(_);
    }

    auto test_hostname() -> void
    {
        const auto hostname {get_hostname(is_verbose)};
        std::cout << "Hostname: "
                  << hostname
                  << std::endl;
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace Test;

    try {
        const auto& context {Context::instance()};
        parse_arguments(argc, argv);

        if (is_verbose) {
            std::cout << context << std::endl;
        }

        test_hostname();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
