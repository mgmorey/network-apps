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
                                  const OptionalString& t_opts) :
    m_argc(t_argc),
    m_argv(t_argv),
    m_opts(t_opts)
{
    const auto span {std::span(t_argv, to_size(t_argc))};
    std::copy(span.begin(), span.end(), std::back_inserter(m_data));

    if (m_opts && m_opts->empty()) {
        m_opts = std::nullopt;
    }
}

auto Network::CommandLine::arguments(std::size_t offset) ->
    Network::CommandLine::ArgumentSpan
{
    return std::span(m_data).subspan(offset);
}

auto Network::CommandLine::arguments(int offset) ->
    Network::CommandLine::ArgumentSpan
{
    return arguments(to_size(offset));
}

auto Network::CommandLine::option() const -> int
{
    if (!m_opts) {
        throw LogicError("No command-line options available to parse");
    }

    return ::getopt(m_argc, m_argv, m_opts->c_str());
}
