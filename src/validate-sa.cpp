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
#include "network/get-sa-size-maximum.h"        // get_sa_size_maximum()
#include "network/get-sa-size-minimum.h"        // get_sa_size_minimum()
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/sa-len-limits.h"              // sa_len_min
#include "network/sa-len-type.h"                // sa_len_type
#include "network/salengtherror.h"              // SaLengthError
#include "network/socket-family-type.h"         // socket_family_type

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNSPEC, sockaddr
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNSPEC, sockaddr
#endif

#include <string>       // std::to_string()
#include <utility>      // std::cmp_greater(), std::cmp_less()

namespace {
    auto validate_length(Network::sa_len_type sa_len,
                         Network::socket_family_type family) -> void
    {
        if (std::cmp_less(sa_len, Network::sa_len_min)) {
            throw Network::SaLengthError(std::to_string(sa_len),
                                         Network::sa_len_min,
                                         Network::sa_len_max);
        }

        const auto size_max {Network::get_sa_size_maximum(family)};
        const auto size_min {Network::get_sa_size_minimum(family)};

        if (std::cmp_less(sa_len, size_min) ||
            std::cmp_greater(sa_len, size_max)) {
            throw Network::SaLengthError(std::to_string(sa_len),
                                         size_min,
                                         size_max);
        }
    }

#ifdef HAVE_SOCKADDR_SA_LEN

    if (family != AF_UNSPEC &&
        (std::cmp_less(sa->sa_len, size_min) ||
         std::cmp_greater(sa->sa_len, size_max))) {
        throw SaLengthError(std::to_string(sa->sa_len),
                            size_min,
                            size_max);
    }

#endif
}

auto Network::validate(const sockaddr *sa, sa_len_type sa_len) -> void
{
    validate_length(sa_len, AF_UNSPEC);
    const socket_family_type family {sa->sa_family};

    if (family != AF_INET && family != AF_INET6) {
        throw FamilyError();
    }

    validate_length(sa_len, family);
}
