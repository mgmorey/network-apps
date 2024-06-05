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

#include "network/validate-sin6.h"              // validate()
#include "network/familyerror.h"                // FamilyError
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN

#ifdef HAVE_SOCKADDR_SA_LEN
#include "network/length-type.h"                // length_type
#include "network/valueerror.h"                 // ValueError
#endif

#ifdef WIN32
#include <winsock2.h>       // AF_INET6
#include <ws2tcpip.h>       // sockaddr_in6
#else
#include <netinet/in.h>     // sockaddr_in6
#include <sys/socket.h>     // AF_INET6
#endif

#ifdef HAVE_SOCKADDR_SA_LEN
#include <utility>      // std::cmp_not_equal()
#endif

auto Network::validate(const sockaddr_in6* sin6) -> const sockaddr_in6*
{
#ifdef HAVE_SOCKADDR_SA_LEN
    if (std::cmp_not_equal(sin6->sin6_len, sizeof *sin6)) {
        throw ValueError<length_type>("sin6_len_type",
                                      sin6->sin6_len,
                                      sizeof *sin6,
                                      sizeof *sin6);
    }
#endif

    if (sin6->sin6_family != AF_INET6) {
        throw FamilyError(sin6->sin6_family);
    }

    return sin6;
}
