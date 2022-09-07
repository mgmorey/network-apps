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

#include "network/version.h"            // Version

namespace Network
{
    struct WindowsVersion
    {
        constexpr WindowsVersion() noexcept = default;

        explicit constexpr WindowsVersion(const Version& t_version) noexcept :
            m_version(t_version)
        {
        }

        constexpr WindowsVersion(const WindowsVersion&) noexcept = default;
        constexpr WindowsVersion(WindowsVersion&&) noexcept = default;
        constexpr ~WindowsVersion() noexcept = default;
        constexpr auto operator=(const WindowsVersion&) noexcept ->
            WindowsVersion& = default;
        constexpr auto operator=(WindowsVersion&&) noexcept ->
            WindowsVersion& = default;

    private:
        Version m_version;
    };
}

#endif
