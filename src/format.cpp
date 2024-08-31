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

#include "network/format.h"             // Format, operator<<()

#include <ostream>      // std::endl, std::ostream
#include <string>       // std::string
#include <string_view>  // std::string_view

Network::Format::Format(std::string_view t_key) noexcept :
    Format(m_delimiter_default,
           m_indent_default,
           t_key)
{
}

Network::Format::Format(std::string::size_type t_indent,
                        std::string_view t_key) noexcept :
    Format(m_delimiter_default,
           t_indent,
           t_key)
{
}

Network::Format::Format(std::string_view t_delimiter,
                        std::string::size_type t_indent,
                        std::string_view t_key) :
    m_delimiter(t_delimiter),
    m_indent(t_indent),
    m_key(t_key)
{
}

auto Network::operator<<(std::ostream& os,
                         const Format& format) -> std::ostream&
{
    if (format.m_indent != std::string::npos) {
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
