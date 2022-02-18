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

#include "network/assert.h"                     // assert()
#include "network/get-sun-path-length.h"        // get_sun_path_length(),
                                                // sockaddr_un,
                                                // std::size_t
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/sun-offsets.h"                // sun_path_offset

#include <algorithm>    // std::max(), std::min()
#include <cstring>      // strnlen()

#ifndef WIN32

auto Network::get_sun_path_length(const sockaddr_un* sun,
                                  std::size_t size) noexcept -> std::size_t
{
    assert(sun_path_offset <= size &&
           size <= sun_path_offset + sizeof sun->sun_path);
    size = std::min(sun_path_offset + sizeof sun->sun_path,
                    std::max(sun_path_offset, size));
#ifdef HAVE_SOCKADDR_SA_LEN
    std::size_t len {sun->sun_len};
    assert(sun_path_offset <= len && len <= size);
    len = std::min(size, std::max(sun_path_offset, len));
    const auto path_len {std::min(size, len) - sun_path_offset};
#else
    const auto path_len {std::min(size - sun_path_offset,
                                 sizeof sun->sun_path)};
#endif
    const auto* path {static_cast<const char*>(sun->sun_path)};
    return strnlen(path, path_len);
}

#endif
