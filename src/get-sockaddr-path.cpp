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

#include "network/get-sockaddr.h"       // Bytes, get_sockaddr(),
                                        // sockaddr_un
#include "network/get-bytespan.h"       // get_bytespan()
#include "network/get-sun-length.h"     // get_sun_length()

#ifndef _WIN32
#include <sys/socket.h>     // AF_LOCAL
#endif

#include <algorithm>    // std::max(), std::min()
#include <cassert>      // assert()

#ifndef _WIN32

auto Network::get_sockaddr(const Pathname& path) -> Network::Bytes
{
    sockaddr_un sun {};
    const auto path_size {std::min(path.size(), sizeof sun.sun_path - 1)};
    const auto min_size {sizeof sun - sizeof sun.sun_path + path_size};
    const auto size {std::max(sizeof(sockaddr), min_size + 1)};
    path.copy(sun.sun_path, path_size);  // NOLINT
    sun.sun_family = AF_LOCAL;
#ifdef HAVE_SOCKADDR_SA_LEN
    sun.sun_len = std::max(sizeof(sockaddr), min_size);
    assert(sun.sun_len == get_sun_length(&sun, sizeof sun));  // NOLINT
#endif
    return get_sockaddr(&sun, size);
}

#endif
