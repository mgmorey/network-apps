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
                                        // get_sun_path(), sockaddr_un
#include "network/get-sa-family.h"      // get_sa_family()
#include "network/get-sun-length.h"     // get_sun_path_length()
#include "network/get-sun-pointer.h"    // get_sun_pointer()
#include "network/sun-offsets.h"        // sun_path_offset

#ifdef _WIN32
#include <winsock2.h>       // AF_UNIX
#else
#include <sys/socket.h>     // AF_UNIX
#endif

#include <algorithm>    // std::max(), std::min()
#include <cassert>      // assert()
#include <cstddef>      // std::size_t
#include <cstring>      // strnlen()

#ifndef _WIN32

auto Network::get_sun_path(const Bytes& addr,
                           const Pathname& path) -> Pathname
{
    assert(get_sa_family(addr) == AF_UNIX);			// NOLINT

    if (addr.size() <= sun_path_offset) {
        return path;
    }

    const auto *const sun {get_sun_pointer(addr)};
    assert(sun->sun_family == AF_UNIX);				// NOLINT
    auto len_max {addr.size() - sun_path_offset};
    auto len {strnlen(sun->sun_path, len_max)};			// NOLINT
    return std::string {sun->sun_path, len};			// NOLINT
}

auto Network::get_sun_path_length(const sockaddr_un* sun,
                                  std::size_t size) -> std::size_t
{
    assert(sun_path_offset <= size &&  				// NOLINT
           size <= sun_path_offset + sizeof sun->sun_path);	// NOLINT
    size = std::min(sun_path_offset + sizeof sun->sun_path,
                    std::max(sun_path_offset, size));
#ifdef HAVE_SOCKADDR_SA_LEN
    std::size_t len {sun->sun_len};
    assert(sun_path_offset <= len && len <= size);		// NOLINT
    len = std::min(size, std::max(sun_path_offset, len));
    const auto max_path_len {std::min(size, len) - sun_path_offset};
#else
    const auto max_path_len {std::min(size - sun_path_offset,
                                      sizeof sun->sun_path)};
#endif
    return strnlen(sun->sun_path, max_path_len);		// NOLINT
}

#endif
