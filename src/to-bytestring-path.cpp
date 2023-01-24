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

#include "network/to-bytestring-path.h"         // ByteString,
                                                // OptionalPathname,
                                                // to_bytestring()
#include "network/get-path-pointer.h"           // get_path_pointer(),
                                                // sockaddr_un
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/sun-offsets.h"                // sun_path_offset
#include "network/to-bytestring-sun.h"          // to_bytestring()
#include "network/to-path-len.h"                // to_path_len()

#ifndef WIN32

#include <sys/socket.h>     // AF_UNIX

auto Network::to_bytestring(const OptionalPathname& pathname) ->
    Network::ByteString
{
    sockaddr_un sun {};
    auto sun_len {sun_path_offset};

    if (pathname) {
        const auto path_len {to_path_len(pathname->length() + 1)};
        pathname->copy(get_path_pointer(&sun), path_len - 1);
        sun_len += path_len;
    }

#ifdef HAVE_SOCKADDR_SA_LEN
    sun.sun_len = sun_len;
#endif
    sun.sun_family = AF_UNIX;
    return to_bytestring(&sun, sun_len);
}

#endif
