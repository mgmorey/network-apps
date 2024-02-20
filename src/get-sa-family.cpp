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

#include "network/get-sa-family.h"              // get_sa_family()
#include "network/bytestring.h"                 // ByteString
#include "network/get-sa-pointer.h"             // get_sa_pointer()
#include "network/sa-len-limits.h"              // sa_len_min,
                                                // sa_len_max
#include "network/salengtherror.h"              // SaLengthError
#include "network/socket-family-type.h"         // socket_family_type

#ifdef WIN32
#include <winsock2.h>       // AF_UNSPEC, sockaddr
#else
#include <sys/socket.h>     // AF_UNSPEC, sockaddr
#endif

#include <string>   // std::to_string()

auto Network::get_sa_family(const ByteString &addr) ->
    Network::socket_family_type
{
    if (addr.size() < sa_len_min ||
        addr.size() > sa_len_max) {
        throw SaLengthError(std::to_string(addr.size()),
                            sa_len_min,
                            sa_len_max);
    }

    const auto* const sa {get_sa_pointer(addr)};
    return sa->sa_family;
}
