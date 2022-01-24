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

#ifndef NETWORK_SA_OFFSETS_H
#define NETWORK_SA_OFFSETS_H

#include "network/os-features.h"        // HAVE_SOCKADDR_SA_LEN, WIN32

#ifdef WIN32
#include <winsock2.h>       // sa_data, sockaddr
#else
#include <sys/socket.h>     // sa_data, sockaddr
#include <sys/un.h>         // sockaddr_un
#endif

#include <cstddef>      // offsetof()

namespace Network
{
#ifdef HAVE_SOCKADDR_SA_LEN
    constexpr auto sa_len_offset {offsetof(sockaddr, sa_len)};
#endif
    constexpr auto sa_family_offset {offsetof(sockaddr, sa_family)};
    constexpr auto sa_data_offset {offsetof(sockaddr, sa_data)};
}

#endif
