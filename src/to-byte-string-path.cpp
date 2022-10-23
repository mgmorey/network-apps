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

#include "network/to-byte-string-path.h"        // ByteString,
                                                // OptionalPathname,
                                                // to_byte_string()
#include "network/get-sun-path-size.h"          // get_sun_path_size()
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#ifndef WIN32
#include "network/sun-offsets.h"                // sun_path_offset
#endif
#include "network/to-byte-string-sun.h"         // sockaddr_un,
                                                // to_byte_string()
#include "network/to-path-len.h"                // to_path_len()

#ifndef WIN32
#include <sys/socket.h>     // AF_LOCAL
#endif

#include <algorithm>    // std::max(), std::min()
#include <sstream>      // std::ostringstream

#ifndef WIN32

auto Network::to_byte_string(const OptionalPathname& pathname) ->
    Network::ByteString
{
    const auto path_len {pathname ? to_path_len(pathname->length()) : 0};
    sockaddr_un sun {};
    auto sun_len_min {sizeof sun - sizeof sun.sun_path + path_len};
#ifdef HAVE_SOCKADDR_SA_LEN
    sun.sun_len = std::max(sun_path_offset, sun_len_min);
#endif
    sun.sun_family = AF_UNIX;

    if (pathname) {
        pathname->copy(static_cast<char*>(sun.sun_path), path_len);
        sun_len_min += sizeof(char);
    }

    const auto size {std::max(sun_path_offset, sun_len_min)};
    return to_byte_string(&sun, size);
}

#endif
