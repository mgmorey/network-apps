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

#include "network/commandline.h"        // CommandLine
#include "network/to-size.h"            // to_size()

#ifdef WIN32
#include <getopt.h>         // getopt(), optarg, opterr, optind
#else
#include <unistd.h>         // getopt(), optarg, opterr, optind
#endif

#include <cstddef>      // std::size_t
#include <span>         // std::span

Network::CommandLine::CommandLine(int argc, char** argv)
{
    if (argc > 0) {
        m_arguments.emplace_back(*argv);
    }
}

auto Network::CommandLine::arguments(int argc, char** argv) ->
    Network::CommandLine::Arguments
{
    const auto args = std::span(argv, to_size(argc));

    for (auto index = optind; index < argc; ++index) {
        const auto i {static_cast<std::size_t>(index)};
        m_arguments.emplace_back(args[i]);
    }

    return m_arguments;
}
