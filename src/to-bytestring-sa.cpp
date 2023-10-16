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

#include "network/to-bytestring-sa.h"           // to_bytestring()
#include "network/bytestring.h"                 // ByteString
#include "network/get-sa-size-maximum.h"        // get_sa_size_maximum()
#include "network/get-sa-size-minimum.h"        // get_sa_size_minimum()
#include "network/logicerror.h"                 // LogicError
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/sa-len-type.h"                // sa_len_type
#include "network/to-bytespan-void.h"           // to_bytespan()
#include "network/to-bytestring-bs.h"           // to_bytestring()
#include "network/to-sa-len.h"                  // to_sa_len()

#include <sstream>      // std::ostringstream

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, sockaddr
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, sockaddr
#endif

auto Network::to_bytestring(const sockaddr* sa,
                            sa_len_type size) -> Network::ByteString
{
    static_cast<void>(to_sa_len(size));

    switch (sa->sa_family) {
    case AF_INET:
    case AF_INET6:
    {
        const auto sa_size_max {get_sa_size_maximum(sa->sa_family)};
        const auto sa_size_min {get_sa_size_minimum(sa->sa_family)};

        if (size < sa_size_min || size > sa_size_max) {
            std::ostringstream oss;
            oss << "IP domain socket length "
                << size
                << " is out of range ["
                << sa_size_min
                << ", "
                << sa_size_max
                << "]";
            throw LogicError(oss.str());
        }

        break;
    }
    default:
        throw LogicError("Invalid IP domain socket address");
    }

#ifdef HAVE_SOCKADDR_SA_LEN
    if (sa->sa_len != size) {
        std::ostringstream oss;
        oss << "Stored IP domain socket length "
            << static_cast<unsigned>(sa->sa_len)
            << " differs from actual length "
            << size;
        throw LogicError(oss.str());
    }
#endif

    return to_bytestring(to_bytespan(sa, size));
}
