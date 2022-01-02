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
#include "network/sun-offsets.h"        // sun_path_offset

#ifndef _WIN32
#include <sys/socket.h>     // AF_LOCAL
#endif

#include <algorithm>    // std::max(), std::min()

#ifndef _WIN32

auto Network::get_sockaddr(const Pathname& path) -> Network::Bytes
{
    sockaddr_un sun {};
    const auto path_len_max {std::min(path.size(), sizeof sun.sun_path - 1)};
    auto sun_len_min {sizeof sun - sizeof sun.sun_path + path_len_max};
#ifdef HAVE_SOCKADDR_SA_LEN
    sun.sun_len = std::max(sun_path_offset, sun_len_min);
#endif
    sun.sun_family = AF_LOCAL;

    if (path.has_value()) {
        path.value().copy(sun.sun_path, path_len_max);  // NOLINT
        sun_len_min += sizeof(char);
    }

    const auto size {std::max(sun_path_offset, sun_len_min)};
    return get_sockaddr(&sun, size);
}

#endif
