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

#include "network/get-option.h"         // Arguments, get_optarg(),
                                        // get_opterr, get_optind(),
                                        // get_option(), get_optopt()
#include "network/logicerror.h"         // LogicError

#ifdef WIN32
#include <getopt.h>         // getopt(), optarg, opterr, optind
#else
#include <unistd.h>         // getopt(), optarg, opterr, optind
#endif

auto Network::get_optarg() -> char*
{
    return ::optarg;
}

auto Network::get_opterr() -> int
{
    return ::opterr;
}

auto Network::get_optind() -> int
{
    return ::optind;
}

auto Network::get_option(const Network::Arguments& args,
                         const char* optstring) -> int
{
    if (optstring == nullptr || *optstring == '\0') {
        throw Network::LogicError("No command-line options to parse");
    }

    return ::getopt(static_cast<int>(args.size()), args.data(), optstring);
}

auto Network::get_optopt() -> int
{
    return ::optopt;
}
