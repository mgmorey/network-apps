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

#include "network/buffer.h"             // Buffer, operator<<(),
                                        // std::ostream, std::string

Network::Buffer::Buffer(std::string::size_type t_size) :
    m_value(t_size, '\0')
{
}

Network::Buffer::operator std::string() const
{
    return m_value.substr(0, m_value.find('\0'));
}

auto Network::Buffer::data() noexcept -> char*
{
    return m_value.data();
}

auto Network::Buffer::size() const noexcept -> std::string::size_type
{
    return m_value.size();
}

auto Network::operator<<(std::ostream& os,
                         const Buffer& buffer) -> std::ostream&
{
    return os << std::string {buffer};
}
