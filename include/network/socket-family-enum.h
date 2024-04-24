// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_SOCKET_FAMILY_ENUM_H
#define NETWORK_SOCKET_FAMILY_ENUM_H

#include "network/socket-family-type.h"         // socket_family_type

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC
#endif

namespace Network
{
    using socket_family_enum = enum : socket_family_type {
        af_unspec = AF_UNSPEC,
#ifndef WIN32
        af_unix = AF_UNIX,
#endif
        af_inet = AF_INET,
        af_inet6 = AF_INET6
    };
}

#endif
