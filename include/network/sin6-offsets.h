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

#ifndef NETWORK_SIN6_OFFSETS_H
#define NETWORK_SIN6_OFFSETS_H

#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN

#ifdef _WIN32
#include <ws2tcpip.h>       // sockaddr_in6
#else
#include <netinet/in.h>     // sockaddr_in6
#endif

#include <cstddef>      // offsetof()

namespace Network
{
#ifdef HAVE_SOCKADDR_SA_LEN
    constexpr auto sin6_len_offset {offsetof(sockaddr_in6, sin6_len)};
#endif
    constexpr auto sin6_family_offset {offsetof(sockaddr_in6, sin6_family)};
    constexpr auto sin6_port_offset {offsetof(sockaddr_in6, sin6_port)};
    constexpr auto sin6_addr_offset {offsetof(sockaddr_in6, sin6_addr)};
}

#endif
