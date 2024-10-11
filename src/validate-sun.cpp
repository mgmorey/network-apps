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
#include "network/length-type.h"                // length_type
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/to-sun-length.h"              // to_sun_length()

#ifdef HAVE_SOCKADDR_SA_LEN
#include "network/sun-length-limits.h"          // sun_length_max,
                                                // sun_length_min
#include "network/valueerror.h"                 // ValueError
#endif

#include <sys/socket.h>     // AF_UNIX
#include <sys/un.h>         // sockaddr_un

#ifdef HAVE_SOCKADDR_SA_LEN
#include <utility>      // std::cmp_not_equal()
#endif

auto Network::validate(const sockaddr_un* sun,
                       length_type sun_len) -> const sockaddr_un*
{
    sun_len = get_sun_length(sun, to_sun_length(sun_len));

#ifdef HAVE_SOCKADDR_SA_LEN
    if (std::cmp_not_equal(sun->sun_len, sun_len)) {
        throw ValueError<length_type>("sun_len_type",
                                      sun->sun_len,
                                      sun_len,
                                      sun_len);
    }
#endif

    if (sun->sun_family != AF_UNIX) {
        throw FamilyError(sun->sun_family);
    }

#ifndef HAVE_SOCKADDR_SA_LEN
    static_cast<void>(sun_len);
#endif
    return sun;
}

#endif
