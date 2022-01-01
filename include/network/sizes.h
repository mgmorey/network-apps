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

#ifndef NETWORK_SIZES_H
#define NETWORK_SIZES_H

#ifdef _WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr
#include <sys/un.h>         // sockaddr_un
#endif

#include <algorithm>    // std::max()
#include <cstddef>      // std::size_t

namespace Network
{
    constexpr auto ss_size {sizeof(sockaddr_storage)};
#ifdef _WIN32
    constexpr auto sun_size {static_cast<std::size_t>(0)};
#else
    constexpr auto sun_size {sizeof(sockaddr_un)};
#endif
    constexpr auto sockaddr_size_max {std::max(ss_size, sun_size)};
}

#endif
