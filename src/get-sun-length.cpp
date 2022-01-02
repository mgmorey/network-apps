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

#include "network/get-sun-length.h"     // Bytes, get_sun_length(),
                                        // sockaddr_un
#include "network/get-sun-path.h"       // get_sun_path_length()
#include "network/sa-sizes.h"           // sa_size

#ifndef _WIN32
#include <sys/socket.h>     // sockaddr
#endif

#include <algorithm>    // std::max(), std::min()
#include <cstddef>      // std::size_t

#ifndef _WIN32

#ifdef HAVE_SOCKADDR_SA_LEN

auto Network::get_sun_length(const sockaddr_un* sun,
                             std::size_t size) -> std::size_t
{
    const auto path_len {get_sun_path_length(sun, size)};
    const auto min_size {sizeof *sun - sizeof sun->sun_path + path_len};
    return std::max(sa_size, min_size);
}

#endif

#endif
