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
#include "network/logicerror.h"                 // LogicError
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/sa-len-type.h"                // sa_len_type
#include "network/sa-offsets.h"                 // sa_data_offset
#include "network/to-bytespan-void.h"           // to_bytespan()
#include "network/to-bytestring-bs.h"           // to_bytestring()
#include "network/to-sa-len.h"                  // to_sa_len()

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, sockaddr
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, sockaddr
#endif

#include <sstream>      // std::ostringstream

auto Network::to_bytestring(const sockaddr* sa,
                            sa_len_type sa_len) -> Network::ByteString
{
    if (sa_len < sa_data_offset ||
        (sa->sa_family != AF_INET &&
         sa->sa_family != AF_INET6)) {
        throw LogicError("Invalid IP domain socket address");
    }

    sa_len = to_sa_len(sa, sa_len);

#ifdef HAVE_SOCKADDR_SA_LEN
    if (sa->sa_len != sa_len) {
        std::ostringstream oss;
        oss << "Stored IP domain socket length "
            << static_cast<size_t>(sa->sa_len)
            << " differs from actual length "
            << static_cast<size_t>(sa_len);
        throw LogicError(oss.str());
    }
#endif

    return to_bytestring(to_bytespan(sa, sa_len));
}
