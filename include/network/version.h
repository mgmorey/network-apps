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
#include <string>      // std::string

namespace Network
{
    struct Version
    {
        constexpr Version() noexcept = default;

        constexpr Version(int t_major, int t_minor) noexcept :
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

        explicit operator std::string() const;

        [[nodiscard]] constexpr auto major() const noexcept -> int
        {
            return m_major;
        }

        [[nodiscard]] constexpr auto minor() const noexcept -> int
        {
            return m_minor;
        }

    private:
        int m_major {0};
        int m_minor {0};
    };

    extern auto operator<<(std::ostream& os,
                           Version version) -> std::ostream&;
}

#endif
