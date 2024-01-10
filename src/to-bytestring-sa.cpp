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
    const auto sa_len {to_sa_len(size, sa->sa_family)};

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
