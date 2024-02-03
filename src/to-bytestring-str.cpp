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

#include "network/to-bytestring-str.h"          // to_bytestring()
#include "network/bytestring.h"                 // ByteString
#include "network/get-path-pointer-sun.h"       // get_path_pointer()
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/sun-len-limits.h"             // sun_len_min
#include "network/sun-len-type.h"               // sun_len_type
#include "network/to-bytestring.h"              // to_bytestring()
#include "network/to-path-len.h"                // to_path_len()

#ifndef WIN32
#include <sys/un.h>         // sockaddr_un
#endif

#ifndef WIN32

#include <sys/socket.h>     // AF_UNIX

#include <cstring>      // ::strlen(), ::strncpy()

auto Network::to_bytestring(const char* path) ->
    Network::ByteString
{
    sockaddr_un sun {};
    sun_len_type sun_len {sun_len_min};

    if (path != nullptr) {
        const auto path_len{to_path_len(::strlen(path) + 1)};
        ::strncpy(get_path_pointer(&sun), path, path_len - 1);
        sun_len += path_len;
    }

    sun.sun_family = AF_UNIX;
#ifdef HAVE_SOCKADDR_SA_LEN
    sun.sun_len = sun_len;
#endif
    return to_bytestring(&sun, sun_len);
}

#endif
