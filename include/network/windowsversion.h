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

#ifdef WIN32

#include <winsock2.h>       // WSADATA, WSAEFAULT, WSAEPROCLIM,
                            // WSASYSNOTREADY, WSAVERNOTSUPPORTED,
                            // WSAStartup()
#include <windows.h>        // WORD

#include <utility>      // std::move()

namespace Network
{
    struct WindowsVersion :
        public Version
    {
        explicit constexpr WindowsVersion(WORD t_version) noexcept :
            Version(t_version % m_radix, t_version / m_radix)
        {
        }

        explicit constexpr WindowsVersion(const Version& t_version) noexcept :
            Version(t_version)
        {
        }

        explicit constexpr WindowsVersion(Version&& t_version) noexcept :
            Version(std::move(t_version))
        {
        }

        explicit constexpr operator WORD() const noexcept
        {
            return minor() * m_radix + major();
        }

    private:
        static constexpr auto m_radix {0x100};
    };
}

#endif
#endif
