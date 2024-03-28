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
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/sa-len-type.h"                // sa_len_type
#include "network/to-sa-len.h"                  // to_sa_len()

#ifdef HAVE_SOCKADDR_SA_LEN
#include "network/sa-len-limits.h"              // sa_len_max,
                                                // sa_len_min
#include "network/salengtherror.h"              // SaLengthError
#endif

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNSPEC, sockaddr,
                            // sockaddr_in
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNSPEC, sockaddr
#include <sys/un.h>         // sockaddr_un
#endif

#include <string>       // std::to_string()
#include <utility>      // std::cmp_not_equal()

namespace {
    using Network::FamilyError;
#ifdef HAVE_SOCKADDR_SA_LEN
    using Network::SaLengthError;
    using Network::sa_len_max;
    using Network::sa_len_min;
#endif
    using Network::sa_len_type;
    using Network::to_sa_len;

    auto validate_sa(const sockaddr* sa, sa_len_type size) -> void
    {
        const auto sa_len {to_sa_len(size)};
#ifdef HAVE_SOCKADDR_SA_LEN

        if (std::cmp_not_equal(sa->sa_len, 0) &&
            std::cmp_not_equal(sa->sa_len, sa_len)) {
          throw SaLengthError(std::to_string(sa->sa_len),
                              sa_len_min,
                              sa_len_max);
        }

#else
        static_cast<void>(sa_len);
#endif

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
    }
}

auto Network::validate(const sockaddr* sa, sa_len_type sa_len) ->
    const sockaddr*
{
    validate_sa(sa, sa_len);
    return sa;
}

auto Network::validate(sockaddr* sa, sa_len_type sa_len) ->
    sockaddr*
{
    validate_sa(sa, sa_len);
    return sa;
}
