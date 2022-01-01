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

#include "network/format.h"     // Format, std::ostream, std::string

#include <utility>      // std::move()

Network::Format::Format(std::string t_delimiter, int t_indent,
                        std::string t_key) :
    m_delimiter(std::move(t_delimiter)),
    m_indent(t_indent),
    m_key(std::move(t_key))
{
}

auto Network::operator<<(std::ostream& os,
                         const Format& format) -> std::ostream&
{
    if (format.m_indent >= 0) {
        os << format.m_delimiter;

        if (format.m_indent > 0) {
            os << std::endl
               << std::string(format.m_indent, ' ');
        }
    }

    if (!format.m_key.empty()) {
        os << format.m_key
           << '=';
    }

    return os;
}
