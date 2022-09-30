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
#include "network/get-arguments.h"      // get_arguments()
#include "network/get-option.h"         // get_optind()
#include "network/to-size.h"            // to_size()

Network::Arguments::Arguments(std::size_t t_argc, char** t_argv) :
    m_args(get_arguments(t_argc, t_argv))
{
}

Network::Arguments::Arguments(int t_argc, char** t_argv) :
    m_args(get_arguments(t_argc, t_argv))
{
}

auto Network::Arguments::operator[](std::size_t t_offset) const ->
    Network::Argument
{
    if (t_offset >= m_args.size()) {
        return nullptr;
    }

    return m_args[t_offset];
}

auto Network::Arguments::operator[](int t_offset) const ->
    Network::Argument
{
    return (*this)[to_size(t_offset)];
}

auto Network::Arguments::arguments() -> Network::ArgumentSpan
{
    return m_args;
}

auto Network::Arguments::optional() -> Network::ArgumentSpan
{
    return m_args.subspan(1, to_size(get_optind()) - 1);
}

auto Network::Arguments::required() -> Network::ArgumentSpan
{
    return m_args.subspan(to_size(get_optind()));
}
