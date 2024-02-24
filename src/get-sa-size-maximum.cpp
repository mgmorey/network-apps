// Copyright (C) 2023  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/get-sa-size-maximum.h"        // get_sa_size_maximum()
#include "network/sa-len-limits.h"              // sa_len_max
#include "network/sin-sizes.h"                  // sin_size
#include "network/sin6-sizes.h"                 // sin6_size
#include "network/socket-family-type.h"         // socket_family_type
#ifndef WIN32
#include "network/sun-sizes.h"                  // sun_size
#endif

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC
#endif

#include <cstddef>      // std::size_t

auto Network::get_sa_size_maximum(socket_family_type family) noexcept ->
    std::size_t
{
    switch (family) {
    case AF_UNSPEC:
        return sa_len_max;
#ifndef WIN32
    case AF_UNIX:
        return sun_size;
#endif
    case AF_INET:
        return sin_size;
    case AF_INET6:
        return sin6_size;
    default:
        return sa_len_max;
    }
}
