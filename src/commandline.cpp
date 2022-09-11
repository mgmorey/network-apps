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

#include <cstddef>      // std::size_t
#include <span>         // std::span

Network::CommandLine::CommandLine(int t_argc,
                                  char* const* t_argv,
                                  const OptionalString& t_options) :
    m_argc(t_argc),
    m_argv(t_argv),
    m_options(t_options)
{
    if (m_argc < 0 || m_argv == nullptr) {
        throw LogicError("No command-line arguments available to parse");
    }
}

auto Network::CommandLine::arguments() const ->
    Network::CommandLine::Arguments
{
    Arguments args;
    const auto arg_span = std::span(m_argv, to_size(m_argc));
    args.emplace_back(arg_span[0]);

    for (auto index = optind; index < m_argc; ++index) {
        const auto i {static_cast<std::size_t>(index)};
        args.emplace_back(arg_span[i]);
    }

    return args;
}

auto Network::CommandLine::option() const -> int
{
    if (!m_options) {
        throw LogicError("No command-line options available to parse");
    }

    return ::getopt(m_argc, m_argv, m_options->c_str());
}
