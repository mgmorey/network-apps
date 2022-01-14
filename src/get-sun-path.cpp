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

#include "network/get-sun-path.h"       // Bytes, OptionalPathname
                                        // get_sun_path()
#include "network/get-sa-family.h"      // get_sa_family()
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
                           const OptionalPathname& pathname) -> OptionalPathname
{
    assert(get_sa_family(addr) == AF_UNIX);			// NOLINT

    if (addr.size() <= sun_path_offset) {
        return pathname;
    }

    const auto *const sun {get_sun_pointer(addr)};
    assert(sun->sun_family == AF_UNIX);				// NOLINT
    auto len_max {addr.size() - sun_path_offset};
    auto len {strnlen(sun->sun_path, len_max)};			// NOLINT
    const std::string result {sun->sun_path, len};		// NOLINT
    return static_cast<OptionalPathname>(result);
}

#endif
