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

#include "network/validate-sa.hpp"      // validate()
#include "network/familyerror.hpp"      // FamilyError
#include "network/length-type.hpp"      // length_type
#include "network/os-features.hpp"      // HAVE_SOCKADDR_SA_LEN
#include "network/to-sa-length.hpp"     // to_sa_length()

#ifdef HAVE_SOCKADDR_SA_LEN
#include "network/valueerror.hpp"       // ValueError
#endif

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNIX, sockaddr
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNIX, sockaddr
#endif

#ifdef HAVE_SOCKADDR_SA_LEN
#include <utility>      // std::cmp_not_equal()
#endif

auto Network::validate(const sockaddr* sa,
                       [[maybe_unused]] length_type sa_len) -> const sockaddr*
{
    static_cast<void>(to_sa_length(sa_len));

#ifdef HAVE_SOCKADDR_SA_LEN
    if (std::cmp_not_equal(sa->sa_len, sa_len)) {
        throw ValueError<length_type>("sa_length_type",
                                      sa->sa_len,
                                      sa_len,
                                      sa_len);
    }
#endif

    switch (sa->sa_family) {
    case AF_INET:
    case AF_INET6:
#ifndef WIN32
    case AF_UNIX:
#endif
        break;
    default:
        throw FamilyError(sa->sa_family);
    }

    return sa;
}
