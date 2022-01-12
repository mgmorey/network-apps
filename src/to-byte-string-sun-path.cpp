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

#include "network/to-byte-string-sun.h" // ByteString,
                                        // OptionalPathname,
                                        // sockaddr_un,
                                        // to_byte_string()
#ifndef _WIN32
#include "network/sun-offsets.h"        // sun_path_offset
#endif

#ifndef _WIN32
#include <sys/socket.h>     // AF_LOCAL
#endif

#include <algorithm>    // std::max(), std::min()

#ifndef _WIN32

auto Network::to_byte_string(const OptionalPathname& pathname) ->
    Network::ByteString
{
    sockaddr_un sun {};
    const auto path_len {pathname ? pathname->length() : 0};
    const auto path_len_max {std::min(path_len, sizeof sun.sun_path - 1)};
    auto sun_len_min {sizeof sun - sizeof sun.sun_path + path_len_max};
#ifdef HAVE_SOCKADDR_SA_LEN
    sun.sun_len = std::max(sun_path_offset, sun_len_min);
#endif
    sun.sun_family = AF_LOCAL;

    if (pathname) {
        pathname->copy(sun.sun_path, path_len_max);  // NOLINT
        sun_len_min += sizeof(char);
    }

    const auto size {std::max(sun_path_offset, sun_len_min)};
    return to_byte_string(&sun, size);
}

#endif
