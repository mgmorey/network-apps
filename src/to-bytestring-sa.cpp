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

#include "network/to-bytestring-sa.h"   // ByteString, sa_len_type,
                                        // sockaddr, to_bytestring()
#include "network/logicerror.h"         // LogicError
#include "network/os-features.h"        // HAVE_SOCKADDR_SA_LEN
#include "network/salengtherror.h"      // SaLengthError
#include "network/sin-sizes.h"          // sin_size
#include "network/sin6-sizes.h"         // sin6_size
#include "network/to-bytespan-void.h"   // to_bytespan()
#include "network/to-bytestring-bs.h"   // to_bytestring()
#include "network/to-sa-len.h"          // to_sa_len()

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_LOCAL, AF_UNIX
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_LOCAL, AF_UNIX
#endif

auto Network::to_bytestring(const sockaddr* sa,
                            sa_len_type size) -> Network::ByteString
{
    static_cast<void>(to_sa_len(size));

    switch (sa->sa_family) {
    case AF_INET:
        if (size != sin_size) {
            throw SaLengthError(std::to_string(size));
        }

        break;
    case AF_INET6:
        if (size != sin6_size) {
            throw SaLengthError(std::to_string(size));
        }

        break;
    default:
        throw LogicError("Invalid IP domain socket address");
    }

#ifdef HAVE_SOCKADDR_SA_LEN
    if (sa->sa_len != size) {
        throw SaLengthError(std::to_string(sa->sa_len));
    }
#endif

    return to_bytestring(to_bytespan(sa, size));
}
