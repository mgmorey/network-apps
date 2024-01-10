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

#include "network/to-bytestring-sun.h"          // to_bytestring()
#include "network/bytestring.h"                 // ByteString
#include "network/get-sun-length.h"             // get_sun_length()
#include "network/logicerror.h"                 // LogicError
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/sun-len-type.h"               // sun_len_type
#include "network/sun-offsets.h"                // sun_path_offset
#include "network/to-bytespan-void.h"           // to_bytespan()
#include "network/to-bytestring-bs.h"           // to_bytestring()
#include "network/to-sun-len.h"                 // to_sun_len()

#ifndef WIN32
#include <sys/socket.h>     // AF_UNIX
#include <sys/un.h>         // sockaddr_un
#endif

#include <sstream>      // std::ostringstream

#ifndef WIN32

auto Network::to_bytestring(const sockaddr_un* sun,
                            sun_len_type size) -> Network::ByteString
{
    if (size < sun_path_offset || sun->sun_family != AF_UNIX) {
        throw LogicError("Invalid UNIX domain socket address");
    }

    const auto sun_len {get_sun_length(sun, to_sun_len(size))};

#ifdef HAVE_SOCKADDR_SA_LEN
    if (sun->sun_len != sun_len) {
        std::ostringstream oss;
        oss << "Stored UNIX domain socket length "
            << static_cast<size_t>(sun->sun_len)
            << " differs from actual length "
            << static_cast<size_t>(size);
        throw LogicError(oss.str());
    }
#endif

    return to_bytestring(to_bytespan(sun, sun_len));
}

#endif
