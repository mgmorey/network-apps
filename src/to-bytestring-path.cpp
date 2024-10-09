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

#ifndef WIN32

#include "network/to-bytestring-path.h"         // to_bytestring()
#include "network/bytestring.h"                 // ByteString
#include "network/get-path-pointer.h"           // get_path_pointer()
#include "network/get-sun-length.h"             // get_sun_length()
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/sun-offsets.h"                // sun_path_offset
#include "network/to-bytestring-void.h"         // to_bytestring()
#include "network/to-path-length.h"             // to_path_length()

#include <sys/socket.h>     // AF_UNIX
#include <sys/un.h>         // sockaddr_un

#include <string_view>  // std::string_view

auto Network::to_bytestring(const std::string_view& path) -> ByteString
{
    sockaddr_un sun {};
    std::size_t sun_len {sun_path_offset};

    if (path.data() != nullptr) {
        const auto path_len {to_path_length(path.length())};
        auto* sun_path {get_path_pointer(&sun)};
        path.copy(sun_path, path_len);
        sun_len += path_len + 1;
    }

#ifdef HAVE_SOCKADDR_SA_LEN
    sun.sun_len = sun_len;
#endif
    sun.sun_family = AF_UNIX;
    return to_bytestring(&sun, sun_len);
}

#endif
