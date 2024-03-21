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

#ifndef WIN32

#include "network/validate-sun.h"               // validate()
#include "network/familyerror.h"                // FamilyError
#include "network/get-sun-length.h"             // get_sun_length()
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/sun-len-limits.h"             // sun_len_max,
                                                // sun_len_min
#include "network/sun-len-type.h"               // sun_len_type
#include "network/sunlengtherror.h"             // SunLengthError
#include "network/to-sun-len.h"                 // to_sun_len()

#include <sys/socket.h>     // AF_UNIX
#include <sys/un.h>         // sockaddr_un

#include <string>       // std::to_string()
#include <utility>      // std::cmp_greater(), std::cmp_less(),
                        // std::cmp_not_equal()

namespace {
    using Network::FamilyError;
#ifdef HAVE_SOCKADDR_SA_LEN
    using Network::SunLengthError;
#endif
    using Network::get_sun_length;
    using Network::sun_len_max;
    using Network::sun_len_type;
    using Network::to_sun_len;

    auto validate_sun(const sockaddr_un* sun, sun_len_type size) -> void
    {
        const auto sun_len {to_sun_len(get_sun_length(sun, size))};

#ifdef HAVE_SOCKADDR_SA_LEN

        if (std::cmp_not_equal(sun->sun_len, sun_len)) {
            throw SunLengthError(std::to_string(sun_len), sun_len);
        }

#else
        static_cast<void>(sun_len);
#endif

        if (sun->sun_family != AF_UNIX) {
            throw FamilyError(sun->sun_family);
        }
    }
}

auto Network::validate(const sockaddr_un* sun,
                       sun_len_type size) -> const sockaddr_un*
{
    validate_sun(sun, size);
    return sun;
}

auto Network::validate(sockaddr_un* sun,
                       sun_len_type size) -> sockaddr_un*
{
    validate_sun(sun, size);
    return sun;
}

#endif
