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
                                        // get_sun_path_length(),
                                        // sockaddr_un

#ifndef _WIN32
#include <sys/socket.h>     // sockaddr
#endif

#include <algorithm>    // std::max(), std::min()
#include <cassert>      // assert()
#include <cstddef>      // std::size_t
#include <cstring>      // strnlen()

#ifndef _WIN32

#ifdef HAVE_SOCKADDR_SA_LEN

auto Network::get_sun_length(const sockaddr_un* sun,
                             std::size_t size) -> std::size_t
{
    const auto path_len {get_sun_path_length(sun, size)};
    const auto min_size {sizeof *sun - sizeof sun->sun_path + path_len};
    return std::max(sizeof(sockaddr), min_size);
}

#endif

auto Network::get_sun_path_length(const sockaddr_un* sun,
                                  std::size_t size) -> std::size_t
{
    static constexpr auto path_offset {offsetof(sockaddr_un, sun_path)};

    assert(size >= path_offset);  // NOLINT
    assert(size <= path_offset + sizeof sun->sun_path);  // NOLINT
    size = std::max(size, path_offset);
    size = std::min(size, path_offset + sizeof sun->sun_path);
#ifdef HAVE_SOCKADDR_SA_LEN
    std::size_t len {sun->sun_len};
    assert(len >= path_offset);  // NOLINT
    assert(len <= size);  // NOLINT
    len = std::max(len, path_offset);
    len = std::min(len, size);
    const auto max_path_len {std::min(size, len) - path_offset};
#else
    const auto max_path_len {std::min(size - path_offset,
                                      sizeof sun->sun_path)};
#endif
    return strnlen(sun->sun_path, max_path_len);  // NOLINT
}

#endif
