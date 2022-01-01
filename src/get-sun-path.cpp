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

#include "network/get-sun-path.h"       // Bytes, Pathname
                                        // get_sun_path()
#include "network/get-sun-length.h"     // get_sun_path_length(), sockaddr_un
#include "network/get-sun-pointer.h"    // get_sun_pointer()

#include <cstring>      // strnlen()

#include <algorithm>    // std::max(), std::min()
#include <cassert>      // assert()
#include <cstddef>      // std::size_t
#include <cstring>      // strnlen()

#ifndef _WIN32

auto Network::get_sun_path(const Bytes& addr,
                           Pathname path) -> Pathname
{
    const auto *const sun {get_sun_pointer(addr)};

    if (offsetof(sockaddr_un, sun_path) <= addr.size()) {
        const auto size {addr.size() - offsetof(sockaddr_un, sun_path)};
        const auto length {strnlen(sun->sun_path, size)};  // NOLINT
        path = std::string(sun->sun_path, length);  // NOLINT
    }

    return path;
}

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
