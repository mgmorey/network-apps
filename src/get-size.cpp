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

#include "network/get-size.h"           // get_size_max(),
                                        // get_size_min()
#include "network/get-sa-family.h"      // get_sa_family()
#include "network/sizes.h"              // sin_size, sin6_size,
                                        // ss_size
#include "network/sun-len-limits.h"     // sun_len_max, sun_len_min

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_LOCAL, AF_UNIX
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_LOCAL, AF_UNIX
#endif

auto Network::get_size_max(const Network::ByteString& addr) noexcept ->
    std::size_t
{
    const auto family {get_sa_family(addr)};

    switch (family) {
    case AF_UNSPEC:
        return ss_size;
#ifndef WIN32
    case AF_UNIX:
        return sun_len_max;
#endif
    case AF_INET:
        return sin_size;
    case AF_INET6:
        return sin6_size;
    default:
        return 0;
    }
}

auto Network::get_size_min(const Network::ByteString& addr) noexcept ->
    std::size_t
{
    const auto family {get_sa_family(addr)};

    switch (family) {
    case AF_UNSPEC:
        return ss_size;
#ifndef WIN32
    case AF_UNIX:
        return sun_len_min;
#endif
    case AF_INET:
        return sin_size;
    case AF_INET6:
        return sin6_size;
    default:
        return 0;
    }
}
