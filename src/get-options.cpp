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

#include "network/get-options.h"        // ArgumentSpan, Options,
                                        // get_options()
#include "network/get-option.h"         // get_optarg(), get_opterr(),
                                        // get_optind(), get_option(),
                                        // get_optopt()
#include "network/to-character.h"       // to_character()

auto Network::get_options(const ArgumentSpan& args,
                          const char* optstring) -> Network::Options
{
    Options options;
    int opt {};

    while ((opt = get_option(args, optstring)) != -1) {
        const auto optchar {to_character(opt)};
        options[optchar] = get_optarg();
    }

    return options;
}
