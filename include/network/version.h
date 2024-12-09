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

#ifndef NETWORK_VERSION_H
#define NETWORK_VERSION_H

#include <ostream>     // std::ostream

namespace Network
{
    struct Version
    {
        using field_type = unsigned short;

        friend auto operator<<(std::ostream& os,
                               Version version) noexcept -> std::ostream&;

        constexpr Version() noexcept = default;

        constexpr Version(field_type t_major, field_type t_minor) noexcept :
            m_major(t_major),
            m_minor(t_minor)
        {
        }

        constexpr Version(const Version&) noexcept = default;
        constexpr Version(Version&&) noexcept = default;
        constexpr ~Version() noexcept = default;
        constexpr auto operator=(const Version&) noexcept -> Version& = default;
        constexpr auto operator=(Version&&) noexcept -> Version& = default;

        constexpr auto operator<(const Version& t_other) const noexcept -> bool
        {
            return (m_major < t_other.m_major ||
                    m_minor < t_other.m_minor);
        }

        constexpr auto operator>(const Version& t_other) const noexcept -> bool
        {
            return (m_major > t_other.m_major ||
                    m_minor > t_other.m_minor);
        }

        constexpr auto operator==(const Version& t_other) const noexcept -> bool
        {
            return (m_major == t_other.m_major &&
                    m_minor == t_other.m_minor);
        }

        constexpr auto operator!=(const Version& t_other) const noexcept -> bool
        {
            return (m_major != t_other.m_major ||
                    m_minor != t_other.m_minor);
        }

    protected:
        field_type m_major {0U};  // NOLINT
        field_type m_minor {0U};  // NOLINT
    };

    extern auto operator<<(std::ostream& os,
                           Version version) noexcept -> std::ostream&;

    static_assert(Version {} == Version {0, 0});
    static_assert(Version {0, 0} < Version {0, 1});
    static_assert(Version {0, 1} < Version {1, 0});
    static_assert(Version {1, 0} < Version {2, 0});
}

#endif
