// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/validate-sa.h"                // validate()
#include "network/familyerror.h"                // FamilyError
#include "network/sa-len-type.h"                // sa_len_type
#include "network/to-sa-len.h"                  // to_sa_len()

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNSPEC, sockaddr,
                            // sockaddr_in
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNSPEC, sockaddr
#include <sys/un.h>         // sockaddr_un
#endif

auto Network::validate(const sockaddr* sa, sa_len_type sa_len) ->
    const sockaddr*
{
    static_cast<void>(to_sa_len(sa_len));

    switch (sa->sa_family) {
#ifndef WIN32
    case AF_UNIX:
#endif
    case AF_INET:
    case AF_INET6:
    case AF_UNSPEC:
        break;
    default:
        throw FamilyError(sa->sa_family);
    }

    return sa;
}

auto Network::validate(sockaddr* sa, sa_len_type sa_len) ->
    sockaddr*
{
    static_cast<void>(to_sa_len(sa_len));
    return sa;
}
