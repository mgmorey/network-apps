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

#include "network/get-option.h"         // get_optarg(), get_opterr(),
                                        // get_optind(), get_option(),
                                        // get_optopt()
#include "network/argumentspan.h"       // ArgumentSpan
#include "network/logicerror.h"         // LogicError

#ifdef WIN32
#include <getopt.h>         // getopt(), optarg, opterr, optind,
                            // optopt
#else
#include <unistd.h>         // NOLINT getopt(), optarg, opterr,
                            // optind, optopt
#endif

auto Network::get_optarg() noexcept -> char*
{
    return ::optarg;  // NOLINT
}

auto Network::get_opterr() noexcept -> int
{
    return ::opterr;  // NOLINT
}

auto Network::get_optind() noexcept -> int
{
    return ::optind;  // NOLINT
}

auto Network::get_option(const ArgumentSpan& args,
                         const char* optstring) -> int
{
    if (optstring == nullptr || *optstring == '\0') {
        throw LogicError("No command-line options to parse");
    }

    // NOLINTNEXTLINE
    return ::getopt(static_cast<int>(args.size()), args.data(), optstring);
}

auto Network::get_optopt() noexcept -> int
{
    return ::optopt;  // NOLINT
}
