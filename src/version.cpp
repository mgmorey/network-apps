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

#include "network/version.h"            // Version, version_type, operator<<(),
                                        // std::ostream

Network::Version::Version(version_type t_version) noexcept :
    m_value(t_version)
{
}

Network::Version::Version(version_type t_major, version_type t_minor) noexcept :
    m_value(t_minor * m_radix + t_major)
{
}

Network::Version::operator version_type() const noexcept
{
    return m_value;
}

Network::Version::operator std::string() const noexcept
{
    return (std::to_string(m_value % m_radix) + "." +
            std::to_string(m_value / m_radix));
}

auto Network::operator<<(std::ostream& os,
                         const Version& version) -> std::ostream&
{
    return os << static_cast<std::string>(version);
}
