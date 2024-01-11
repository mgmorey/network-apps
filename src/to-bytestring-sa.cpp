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

#include "network/to-bytestring-sa.h"   // to_bytestring()
#include "network/bytestring.h"         // ByteString
#include "network/sa-len-type.h"        // sa_len_type
#include "network/to-bytespan-void.h"   // to_bytespan()
#include "network/to-bytestring-bs.h"   // to_bytestring()
#include "network/to-sa-len.h"          // to_sa_len()
#include "network/validate-sa.h"        // validate()

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, sockaddr
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, sockaddr
#endif

auto Network::to_bytestring(const sockaddr* sa,
                            sa_len_type sa_len) -> Network::ByteString
{
    validate(sa, sa_len);
    return to_bytestring(to_bytespan(sa, to_sa_len(sa, sa_len)));
}
