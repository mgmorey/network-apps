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

#ifndef NETWORK_SIN_OFFSETS_H
#define NETWORK_SIN_OFFSETS_H

#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN

#ifdef _WIN32
#include <winsock2.h>       // sockaddr_in
#else
#include <netinet/in.h>     // sockaddr_in
#endif

#include <cstddef>      // offsetof()

namespace Network
{
#ifdef HAVE_SOCKADDR_SA_LEN
    constexpr auto sin_len_offset {offsetof(sockaddr_in, sin_len)};
#endif
    constexpr auto sin_family_offset {offsetof(sockaddr_in, sin_family)};
    constexpr auto sin_port_offset {offsetof(sockaddr_in, sin_port)};
    constexpr auto sin_addr_offset {offsetof(sockaddr_in, sin_addr)};
}

#endif
