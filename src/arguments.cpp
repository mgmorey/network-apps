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

#include "network/arguments.h"          // Arguments
#include "network/assert.h"             // assert()
#include "network/logicerror.h"         // LogicError
#include "network/to-size.h"            // to_size()

#ifdef USING_GETOPT
#ifdef WIN32
#include <getopt.h>         // getopt(), optarg, opterr, optind
#else
#include <unistd.h>         // getopt(), optarg, opterr, optind
#endif
#endif

auto Network::Arguments::option_index() -> int
{
#ifdef USING_GETOPT
    return ::optind;
#else
    return 1;
#endif
}

Network::Arguments::Arguments(int t_argc, char** t_argv) :
    m_span(std::span(t_argv, to_size(t_argc)))
{
}

auto Network::Arguments::operator[](std::size_t t_offset) const ->
    Network::Arguments::Argument
{
    if (t_offset >= m_span.size()) {
        return nullptr;
    }

    return m_span[t_offset];
}

auto Network::Arguments::operator[](int t_offset) const ->
    Network::Arguments::Argument
{
    return (*this)[to_size(t_offset)];
}

auto Network::Arguments::option(const char* optstring) const -> int
{
    if (optstring == nullptr || *optstring == '\0') {
        throw LogicError("No command-line options available to parse");
    }

#ifdef USING_GETOPT
    const auto optind_begin {::optind};
    const auto opt {::getopt(static_cast<int>(m_span.size()),
                             m_span.data(),
                             optstring)};
    assert(opt == -1 || opt == '?' || optind_begin < ::optind);
    return opt;
#else
    static_cast<void>(optstring);
    return -1;
#endif
}

auto Network::Arguments::span(std::size_t t_offset, std::size_t t_count) ->
    Network::Arguments::ArgumentSpan
{
    return m_span.subspan(t_offset, t_count);
}

auto Network::Arguments::span(int t_offset, std::size_t t_count) ->
    Network::Arguments::ArgumentSpan
{
    if (t_offset == -1) {
        t_offset = option_index();
    }

    return span(to_size(t_offset), t_count);
}
