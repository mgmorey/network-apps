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

#include "network/get-path-bytestring.h"        // ByteString,
                                                // OptionalPathname
                                                // get_path()
#include "network/get-sa-family.h"              // get_sa_family()
#include "network/get-sun-pointer.h"            // get_sun_pointer()
#include "network/sun-offsets.h"                // sun_path_offset

#ifdef WIN32
#include <winsock2.h>       // AF_UNIX
#else
#include <sys/socket.h>     // AF_UNIX
#endif

#include <cstddef>      // std::size_t
#include <cstring>      // strnlen()

auto Network::get_path(const ByteString& addr,
                       const OptionalPathname& path) ->
    OptionalPathname
{
#ifndef WIN32
    if (get_sa_family(addr) != AF_UNIX) {
        return path;
    }

    if (addr.size() <= sun_path_offset) {
        return path;
    }

    const auto *const sun {get_sun_pointer(addr)};
    auto path_len_max {addr.size() - sun_path_offset};
    const auto *c_path {static_cast<const char*>(sun->sun_path)};
    auto path_len {strnlen(c_path, path_len_max)};
    return std::string {c_path, path_len};
#else
    static_cast<void>(addr);
    return path;
#endif
}
