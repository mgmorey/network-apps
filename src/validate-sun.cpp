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

#include "network/validate-sun.h"               // validate()
#include "network/familyerror.h"                // FamilyError
#include "network/get-sun-length.h"             // get_sun_length()
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/sun-len-limits.h"             // sun_len_max,
                                                // sun_len_min
#include "network/sun-len-type.h"               // sun_len_type
#include "network/sunlengtherror.h"             // SunLengthError

#ifndef WIN32
#include <sys/socket.h>     // AF_UNIX
#include <sys/un.h>         // sockaddr_un
#endif

#include <string>       // std::to_string()
#include <utility>      // std::cmp_greater(), std::cmp_less(),
                        // std::cmp_not_equal()

#ifndef WIN32

namespace {
    auto validate_sun(const sockaddr_un* sun, Network::sun_len_type size) ->
        void
    {
        if (std::cmp_less(size, Network::sun_len_min) ||
            std::cmp_greater(size, Network::sun_len_max)) {
            throw Network::SunLengthError(std::to_string(size),
                                          Network::sun_len_max);
        }

        const auto sun_len {Network::get_sun_length(sun, Network::sun_len_max)};

        if (std::cmp_less(sun_len, Network::sun_len_min) ||
            std::cmp_greater(sun_len, Network::sun_len_max)) {
            throw Network::SunLengthError(std::to_string(sun_len),
                                          Network::sun_len_max);
        }

#ifdef HAVE_SOCKADDR_SA_LEN

        if (std::cmp_not_equal(sun->sun_len, sun_len)) {
            throw Network::SunLengthError(std::to_string(sun_len),
                                          sun_len);
        }

#endif

        if (sun->sun_family != AF_UNIX) {
            throw Network::FamilyError();
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
