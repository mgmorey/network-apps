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

#ifndef NETWORK_STARTUP_H
#define NETWORK_STARTUP_H

#include "network/context.h"            // Context
#include "network/optionalversion.h"    // OptionalVersion, Version

#ifdef WIN32
#include <winsock2.h>       // WSADATA, WSAEFAULT, WSAEPROCLIM,
                            // WSASYSNOTREADY, WSAVERNOTSUPPORTED,
                            // WSAStartup()
#include <windows.h>        // WORD
#endif

namespace Network
{
#ifdef WIN32

    static constexpr auto radix {0x100};

    constexpr auto to_integer(const Version& version) -> WORD
    {
        const int value {version.minor() * radix + version.major()};
        return static_cast<WORD>(value);
    }

    constexpr auto to_version(WORD version) -> Version
    {
        const int value {version};
        return Version {value % radix, value / radix};
    }

#endif

    extern auto startup(Context& context, const OptionalVersion& version) -> void;
}

#endif
