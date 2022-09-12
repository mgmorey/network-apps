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
#include "network/logicerror.h"         // LogicError
#include "network/to-size.h"            // to_size()

#ifdef WIN32
#include <getopt.h>         // getopt(), optarg, opterr, optind
#else
#include <unistd.h>         // getopt(), optarg, opterr, optind
#endif

#include <algorithm>    // std::copy()
#include <iterator>     // std::back_inserter()
#include <optional>     // std::nullopt

Network::CommandLine::CommandLine(int t_argc,
                                  char** t_argv,
                                  const OptionalString& t_options) :
    m_args(std::span(t_argv, to_size(t_argc))),
    m_opts(t_options)
{
    if (m_opts && m_opts->empty()) {
        m_opts = std::nullopt;
    }
}

auto Network::CommandLine::arguments() const ->
    Network::CommandLine::Arguments
{
    Arguments args;
    args.emplace_back(m_args[0]);
    const auto tail(m_args.subspan(to_size(optind)));
    std::copy(tail.begin(), tail.end(), std::back_inserter(args));
    return args;
}

auto Network::CommandLine::option() const -> int
{
    if (!m_opts) {
        throw LogicError("No command-line options available to parse");
    }

    const auto argc {static_cast<int>(m_args.size())};
    auto* argv {m_args.data()};
    const auto* options {m_opts->c_str()};
    return ::getopt(argc, argv, options);
}
