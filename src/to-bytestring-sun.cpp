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

#include "network/to-bytestring-sun.h"          // ByteString,
                                                // sockaddr_un,
                                                // sun_len_min,
                                                // to_bytestring()
#include "network/get-path-pointer-sun.h"       // get_path_pointer()
#include "network/logicerror.h"                 // LogicError
#include "network/pathlengtherror.h"            // PathLengthError
#include "network/sunlengtherror.h"             // SunLengthError
#include "network/to-bytespan-void.h"           // to_bytespan()
#include "network/to-bytestring-bs.h"           // to_bytestring()
#include "network/to-sun-len.h"                 // to_sun_len()

#ifndef WIN32
#include <sys/socket.h>     // AF_UNIX
#endif

#include <cstring>      // ::strnlen()

#ifndef WIN32

auto Network::to_bytestring(const sockaddr_un* sun,
                            sun_len_type size) -> Network::ByteString
{
    size = to_sun_len(size);

#ifdef HAVE_SOCKADDR_SA_LEN
    if (sun->sun_len > size) {
        throw SunLengthError(std::to_string(sun->sun_len));
    }
#endif

    if (sun->sun_family != AF_UNIX) {
        throw LogicError("Invalid UNIX domain socket address");
    }

    const auto* const path = get_path_pointer(sun);
    const auto path_len = size - sun_len_min;

    if (path_len > 0 && ::strnlen(path, path_len) >= path_len) {
        throw PathLengthError(std::to_string(path_len));
    }

    return to_bytestring(to_bytespan(sun, size));
}

#endif
