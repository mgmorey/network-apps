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
                                                // to_bytestring()
#include "network/get-path-length.h"            // get_path_length()
#include "network/logicerror.h"                 // LogicError
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/sunlengtherror.h"             // SunLengthError
#include "network/to-bytespan-void.h"           // to_bytespan()
#include "network/to-bytestring-bs.h"           // to_bytestring()
#include "network/to-sun-len.h"                 // to_sun_len()

#ifndef WIN32
#include <sys/socket.h>     // AF_UNIX
#endif

#ifndef WIN32

auto Network::to_bytestring(const sockaddr_un* sun,
                            sun_len_type size) -> Network::ByteString
{
    static_cast<void>(to_sun_len(size));

    if (sun->sun_family != AF_UNIX) {
        throw LogicError("Invalid UNIX domain socket address");
    }

    const auto path_len {get_path_length(sun, size)};

    if (sun_len_min + (path_len == 0 ? 0 : path_len + 1) != size) {
        throw SunLengthError(std::to_string(size));
    }

#ifdef HAVE_SOCKADDR_SA_LEN
    if (sun->sun_len != size) {
        throw SunLengthError(std::to_string(sun->sun_len));
    }
#endif

    return to_bytestring(to_bytespan(sun, size));
}

#endif
