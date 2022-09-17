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
#include "network/commandline.h"        // CommandLine
#include "network/logicerror.h"         // LogicError
#include "network/to-size.h"            // to_size()

#ifdef WIN32
#include <getopt.h>         // getopt(), optarg, opterr, optind
#else
#include <unistd.h>         // getopt(), optarg, opterr, optind
#endif

Network::CommandLine::CommandLine(int t_argc, char** t_argv) :
    m_data(t_argv),
    m_size(to_size(t_argc)),
    m_span(std::span(m_data, m_size))
{
}

auto Network::CommandLine::operator[](std::size_t t_offset) const ->
    Network::CommandLine::Argument
{
    if (t_offset >= m_span.size()) {
        return nullptr;
    }

    return m_span[t_offset];
}

auto Network::CommandLine::operator[](int t_offset) const ->
    Network::CommandLine::Argument
{
    return (*this)[to_size(t_offset)];
}

auto Network::CommandLine::option(const char* t_optstring) const -> int
{
    if (t_optstring == nullptr || *t_optstring == '\0') {
        throw LogicError("No command-line options available to parse");
    }

    const auto optind_begin {optind};
    const auto opt {::getopt(static_cast<int>(m_size), m_data, t_optstring)};
    const auto optind_end {optind};
    assert(opt == -1 || opt == '?' || optind_begin < optind_end);
    return opt;
}

auto Network::CommandLine::span(std::size_t t_offset,
                                std::size_t t_count) ->
    Network::CommandLine::ArgumentSpan
{
    return m_span.subspan(t_offset, t_count);
}

auto Network::CommandLine::span(int t_offset,
                                std::size_t t_count) ->
    Network::CommandLine::ArgumentSpan
{
    if (t_offset == -1) {
        t_offset = optind;
    }

    return span(to_size(t_offset), t_count);
}
