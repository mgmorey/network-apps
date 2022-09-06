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

#include "network/version-type.h"       // version_type

#include <ostream>     // std::ostream
#include <string>      // std::string

namespace Network
{
    class Version
    {
    public:
        static constexpr auto t_radix_default {0x100U};
        static constexpr auto t_value_default {0U};

        constexpr Version() noexcept = default;

        constexpr explicit Version(version_type t_version) noexcept :
            m_value(t_version)
        {
        }

        constexpr Version(version_type t_major, version_type t_minor) noexcept :
            m_value(t_minor * m_radix + t_major)
        {
        }

        constexpr Version(const Version&) noexcept = default;
        constexpr Version(Version&&) noexcept = default;
        constexpr ~Version() noexcept = default;
        constexpr auto operator=(const Version&) noexcept -> Version& = default;
        constexpr auto operator=(Version&&) noexcept -> Version& = default;

        constexpr auto operator<(const Version& t_other) const noexcept -> bool
        {
            return (major() < t_other.major() ||
                    minor() < t_other.minor());
        }

        constexpr auto operator>(const Version& t_other) const noexcept -> bool
        {
            return (major() > t_other.major() ||
                    minor() > t_other.minor());
        }

        constexpr auto operator==(const Version& t_other) const noexcept -> bool
        {
            return m_value == t_other.m_value;
        }

        constexpr auto operator!=(const Version& t_other) const noexcept -> bool
        {
            return m_value != t_other.m_value;
        }

        constexpr explicit operator version_type() const noexcept
        {
            return m_value;
        }

        [[nodiscard]] constexpr auto major() const noexcept -> version_type
        {
            return m_value % m_radix;
        }

        [[nodiscard]] constexpr auto minor() const noexcept -> version_type
        {
            return m_value / m_radix;
        }

        explicit operator std::string() const;

    private:
        static constexpr auto m_radix {t_radix_default};
        version_type m_value {t_value_default};
    };

    extern auto operator<<(std::ostream& os,
                           Version version) -> std::ostream&;
}

#endif
