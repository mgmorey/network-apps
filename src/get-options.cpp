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

#include "network/get-options.h"        // Arguments, get_optarg(),
                                        // get_opterr, get_optind(),
                                        // get_option(), get_optopt()
#include "network/assert.h"             // assert()
#include "network/to-integer.h"         // to_integer()

#include <cctype>       // std::isalpha()

auto Network::get_options(const Network::Arguments& args,
                          const char* optstring) -> Network::Options
{
    Options options;
    int opt {};

    while ((opt = get_option(args, optstring)) != -1) {
        if (std::isalpha(opt) != 0) {
            const auto optch {static_cast<char>(opt)};
            options[optch] = get_optarg();
        }
        else {
            std::abort();
        }
    }

    return options;
}
