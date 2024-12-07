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

#ifndef NETWORK_WINDOWSVERSION_H
#define NETWORK_WINDOWSVERSION_H

#ifdef WIN32

#include "network/version.h"                    // Version

#define WIN32_LEAN_AND_MEAN
#include <windows.h>        // HIBYTE(), LOBYTE(), MAKEWORD(), WORD

#include <utility>      // std::move()

namespace Network
{
    struct WindowsVersion : public Version
    {
        using value_type = WORD;

        explicit constexpr WindowsVersion(const Version& t_version) noexcept :
            Version(t_version)
        {
        }

        explicit constexpr WindowsVersion(Version&& t_version) noexcept :
            Version(std::move(t_version))  // NOLINT
        {
        }

        explicit constexpr WindowsVersion(value_type t_version) noexcept :
            Version(static_cast<field_type>(LOBYTE(t_version)),
                    static_cast<field_type>(HIBYTE(t_version)))
        {
        }

        explicit constexpr operator value_type() const noexcept
        {
            return MAKEWORD(m_major, m_minor);
        }
    };

    static_assert(WORD {WindowsVersion(Version {0, 0})} == 0x0000U);
    static_assert(WORD {WindowsVersion(Version {0, 1})} == 0x0100U);
    static_assert(WORD {WindowsVersion(Version {1, 0})} == 0x0001U);
    static_assert(Version {WindowsVersion(0x0000U)} == Version {0, 0});
    static_assert(Version {WindowsVersion(0x0100U)} == Version {0, 1});
    static_assert(Version {WindowsVersion(0x0001U)} == Version {1, 0});
}

#endif

#endif
