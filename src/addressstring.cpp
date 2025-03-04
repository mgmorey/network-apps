// Copyright (C) 2023  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/addressstring.h"      // AddressString, operator<<()
#include "network/format-span-byte.h"   // format()

#include <cstddef>      // std::byte
#include <ostream>      // std::ostream
#include <span>         // std::span
#include <string>       // std::string

Network::AddressString::AddressString(std::span<const std::byte> t_bs) :
    m_bs(t_bs)
{
}

Network::AddressString::operator std::string() const
{
    return format(m_bs, m_str);
}

auto Network::operator<<(std::ostream& os,
                         const AddressString& as) -> std::ostream&
{
    os << std::string(as);
    return os;
}
