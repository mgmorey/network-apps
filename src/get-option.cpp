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

#include "network/get-option.h"         // Arguments, get_option()
#include "network/assert.h"             // assert()
#include "network/logicerror.h"         // LogicError

#ifdef USING_GETOPT
#ifdef WIN32
#include <getopt.h>         // getopt(), optarg, opterr, optind
#else
#include <unistd.h>         // getopt(), optarg, opterr, optind
#endif
#endif

auto Network::get_option(const Network::Arguments& args,
                         const char* optstring) -> int
{
    if (optstring == nullptr || *optstring == '\0') {
        throw Network::LogicError("No command-line options to parse");
    }

#ifdef USING_GETOPT
    const auto optind_begin {::optind};
    const auto opt {::getopt(static_cast<int>(args.size()),
                             args.data(),
                             optstring)};
    assert(opt == -1 || opt == '?' || optind_begin < ::optind);
    return opt;
#else
    static_cast<void>(optstring);
    return -1;
#endif
}
