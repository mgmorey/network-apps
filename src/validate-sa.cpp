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
#include "network/get-sa-size-maximum.h"        // get_sa_size_maximum()
#include "network/get-sa-size-minimum.h"        // get_sa_size_minimum()
#include "network/logicerror.h"                 // LogicError
#include "network/sa-len-type.h"                // sa_len_type
#include "network/salengtherror.h"              // SaLengthError
#include "network/socket-family-type.h"         // socket_family_type

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, sockaddr
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, sockaddr
#endif

#include <string>       // std::to_string()
#include <utility>      // cmp_greater(), cmp_less()

namespace {
    auto validate_length(Network::sa_len_type sa_len,
                         Network::socket_family_type family) -> void
    {
        const auto size_max {Network::get_sa_size_maximum(family)};
        const auto size_min {Network::get_sa_size_minimum(family)};

        if (std::cmp_less(sa_len, size_min) ||
            std::cmp_greater(sa_len, size_max)) {
            const auto str {std::to_string(sa_len)};
            throw Network::SaLengthError(str, size_min, size_max);
        }
    }
}

auto Network::validate(const sockaddr *sa, sa_len_type sa_len) -> void
{
    validate_length(sa_len, 0);
    const socket_family_type family {sa->sa_family};

    if (family != AF_INET && family != AF_INET6) {
        throw LogicError("Invalid IP domain socket address");
    }

    validate_length(sa_len, family);
}
