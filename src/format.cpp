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

#include "network/format.h"             // Format, operator<<(),
                                        // std::endl, std::ostream,
                                        // std::string

#include <utility>      // std::move()

Network::Format::Format(const std::string& t_key) noexcept :
    Format(m_delimiter_default,
           m_indent_default,
           t_key)
{
}

Network::Format::Format(std::string&& t_key) noexcept :
    Format(m_delimiter_default,
           m_indent_default,
           t_key)
{
}

Network::Format::Format(std::string::size_type t_indent,
                        const std::string& t_key) noexcept :
    Format(m_delimiter_default,
           t_indent,
           t_key)
{
}

Network::Format::Format(std::string::size_type t_indent,
                        std::string&& t_key) noexcept :
    Format(m_delimiter_default,
           t_indent,
           t_key)
{
}

Network::Format::Format(const std::string& t_delimiter,
                        std::string::size_type t_indent,
                        const std::string& t_key) noexcept :
    m_delimiter(t_delimiter),
    m_indent(t_indent),
    m_key(t_key)
{
}

Network::Format::Format(const std::string& t_delimiter,
                        std::string::size_type t_indent,
                        std::string&& t_key) noexcept :
    m_delimiter(t_delimiter),
    m_indent(t_indent),
    m_key(std::move(t_key))
{
}

Network::Format::Format(std::string&& t_delimiter,
                        std::string::size_type t_indent,
                        const std::string& t_key) noexcept :
    m_delimiter(std::move(t_delimiter)),
    m_indent(t_indent),
    m_key(t_key)
{
}

Network::Format::Format(std::string&& t_delimiter,
                        std::string::size_type t_indent,
                        std::string&& t_key) noexcept :
    m_delimiter(std::move(t_delimiter)),
    m_indent(t_indent),
    m_key(std::move(t_key))
{
}

auto Network::operator<<(std::ostream& os,
                         const Format& format) noexcept -> std::ostream&
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
