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
#include "network/sa-len-limits.h"              // sa_len_max,
                                                // sa_len_min
#include "network/sa-len-type.h"                // sa_len_type
#include "network/salengtherror.h"              // SaLengthError
#include "network/validate-sin6.h"              // validate()
#include "network/validate-sin.h"               // validate()


#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNSPEC, sockaddr,
                            // sockaddr_in
#include <ws2tcpip.h>       // sockaddr_in6
#else
#include <netinet/in.h>     // sockaddr_in, sockaddr_in6
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNSPEC, sockaddr
#endif

#include <string>       // std::to_string()
#include <utility>      // std::cmp_greater(), std::cmp_less()

auto Network::validate(const sockaddr* sa, sa_len_type sa_len) ->
    const sockaddr*
{
    if (std::cmp_less(sa_len, Network::sa_len_min) ||
        std::cmp_greater(sa_len, Network::sa_len_max)) {
        throw Network::SaLengthError(std::to_string(sa_len),
                                     Network::sa_len_min,
                                     Network::sa_len_max);
    }

    const void* pointer {sa};

    switch (sa->sa_family) {
#ifndef WIN32
    case AF_UNIX:
        break;
#endif
    case AF_INET:
        validate(static_cast<const sockaddr_in*>(pointer));
        break;
    case AF_INET6:
        validate(static_cast<const sockaddr_in6*>(pointer));
        break;
    case AF_UNSPEC:
        break;
    default:
        throw FamilyError();
    }

    return sa;
}

auto Network::validate(sockaddr* sa, sa_len_type sa_len) ->
    sockaddr*
{
    if (std::cmp_less(sa_len, Network::sa_len_min) ||
        std::cmp_greater(sa_len, Network::sa_len_max)) {
        throw Network::SaLengthError(std::to_string(sa_len),
                                     Network::sa_len_min,
                                     Network::sa_len_max);
    }

    return sa;
}
