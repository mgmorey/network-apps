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

#include "network/validate-bs.h"                // validate
#include "network/bytestring.h"                 // ByteString
#include "network/get-sa-family.h"              // get_sa_family()
#include "network/get-sa-size-maximum.h"        // get_sa_size_maximum()
#include "network/get-sa-size-minimum.h"        // get_sa_size_minimum()
#include "network/logicerror.h"                 // LogicError
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/salengtherror.h"              // SaLengthError

#ifdef HAVE_SOCKADDR_SA_LEN
#include "network/get-sa-length.h"              // get_sa_length()
#endif

#ifndef WIN32
#include "network/get-sun-length.h"             // get_sun_length()
#include "network/get-sun-pointer.h"            // get_sun_pointer()
#include "network/sun-len-limits.h"             // sun_len_max
#include "network/sunlengtherror.h"             // SunLengthError
#endif

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC
#endif

#include <string>       // std::to_string()
#include <utility>      // std::cmp_greater(), std::cmp_less(),
                        // std::cmp_not_equal()

auto Network::validate(const ByteString& addr) -> void
{
    const auto family {get_sa_family(addr)};

    switch (family) {
#ifndef WIN32
    case AF_UNIX:
#endif
    case AF_INET:
    case AF_INET6:
        break;
    default:
        throw LogicError("Invalid socket address family");
    }

    const auto size {addr.size()};
    const auto size_max {get_sa_size_maximum(family)};
    const auto size_min {get_sa_size_minimum(family)};

    if (size < size_min || size > size_max) {
        throw SaLengthError(std::to_string(size), size_min, size_max);
    }

#ifdef HAVE_SOCKADDR_SA_LEN

    const auto sa_len {get_sa_length(addr)};

    if (std::cmp_not_equal(sa_len, size)) {
        throw SaLengthError(std::to_string(sa_len), size, size);
    }

#endif
#ifndef WIN32

    if (family == AF_UNIX) {
        const auto* const sun {get_sun_pointer(addr)};
        const auto sun_len {get_sun_length(sun, sun_len_max)};

        if (std::cmp_less(sun_len, size_min) ||
            std::cmp_greater(sun_len, size_max)) {
            throw SunLengthError(std::to_string(sun_len), size_max);
        }
    }

#endif
}
