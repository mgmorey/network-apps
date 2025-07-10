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

#ifndef _WIN32

#include "network/validate-sun.hpp"     // validate()
#include "network/familyerror.hpp"      // FamilyError
#include "network/length-type.hpp"      // length_type

#ifdef HAVE_SOCKADDR_SA_LEN
#include "network/get-sun-length.hpp"   // get_sun_length()
#include "network/os-features.hpp"      // HAVE_SOCKADDR_SA_LEN
#include "network/sun-length-limits.hpp"// sun_length_max,
                                        // sun_length_min
#include "network/to-sun-length.hpp"    // to_sun_length()
#include "network/valueerror.hpp"       // ValueError
#endif

#include <sys/socket.h>     // AF_UNIX
#include <sys/un.h>         // sockaddr_un

#ifdef HAVE_SOCKADDR_SA_LEN
#include <utility>      // std::cmp_not_equal()
#endif

auto Network::validate(const sockaddr_un* sun,
                       [[maybe_unused]] length_type sun_len) ->
    const sockaddr_un*
{
#ifdef HAVE_SOCKADDR_SA_LEN
    sun_len = get_sun_length(sun, to_sun_length(sun_len));

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

    return sun;
}

#endif
