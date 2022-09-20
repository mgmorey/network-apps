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
#include "network/get-option.h"         // get_optind()
#include "network/to-size.h"            // to_size()

Network::Arguments::Arguments(std::size_t t_argc, char** t_argv) :
    m_span(std::span(t_argv, t_argc))
{
}

Network::Arguments::Arguments(int t_argc, char** t_argv) :
    Arguments(to_size(t_argc), t_argv)
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

auto Network::Arguments::data() const -> Argument const*
{
    return m_span.data();
}

auto Network::Arguments::size() const -> std::size_t
{
    return m_span.size();
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
        t_offset = get_optind();
    }

    return span(to_size(t_offset), t_count);
}
