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

#include "network/version.h"    // Version

#define WIN32_LEAN_AND_MEAN
#include <windows.h>        // HIBYTE(), LOBYTE(), MAKEWORD(), WORD

#include <utility>      // std::move()

namespace Network
{
    struct WindowsVersion : public Version
    {
        using value_type = WORD;

        static constexpr Version latest {2, 2};

        constexpr WindowsVersion(const WindowsVersion&) noexcept = default;
        constexpr WindowsVersion(WindowsVersion&&) noexcept = default;
        constexpr ~WindowsVersion() noexcept = default;
        constexpr auto operator=(const WindowsVersion&) noexcept ->
            WindowsVersion& = default;
        constexpr auto operator=(WindowsVersion&&) noexcept ->
            WindowsVersion& = default;

        constexpr WindowsVersion(const Version& t_version) noexcept :  //NOLINT
            Version(t_version)
        {
        }

        constexpr WindowsVersion(Version&& t_version) noexcept :  //NOLINT
            Version(std::move(t_version))  // NOLINT
        {
        }

        constexpr WindowsVersion(value_type t_value) noexcept :  //NOLINT
            Version(static_cast<field_type>(LOBYTE(t_value)),
                    static_cast<field_type>(HIBYTE(t_value)))
        {
        }

        constexpr operator value_type() const noexcept  //NOLINT
        {
            return MAKEWORD(m_major, m_minor);
        }
    };

    static_assert(WindowsVersion(Version {0, 0}) == 0x0000U);  //NOLINT
    static_assert(WindowsVersion(Version {0, 1}) == 0x0100U);  //NOLINT
    static_assert(WindowsVersion(Version {1, 0}) == 0x0001U);  //NOLINT
    static_assert(WindowsVersion(0x0000U) == Version {0, 0});  //NOLINT
    static_assert(WindowsVersion(0x0100U) == Version {0, 1});  //NOLINT
    static_assert(WindowsVersion(0x0001U) == Version {1, 0});  //NOLINT
}

#endif

#endif
