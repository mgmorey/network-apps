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

#include "network/version.h"            // Version, version_type,
                                        // operator<<(), std::ostream
#include "network/string-null.h"        // string_null

#include <string>       // std::to_string

Network::Version::operator std::string() const noexcept
{
    if (!static_cast<bool>(m_value)) {
        return string_null;
    }

    const auto major_version {std::to_string(major())};
    const auto minor_version {std::to_string(minor())};
    return major_version + "." + minor_version;
}

auto Network::operator<<(std::ostream& os,
                         Version version) -> std::ostream&
{
    return os << static_cast<std::string>(version);
}
