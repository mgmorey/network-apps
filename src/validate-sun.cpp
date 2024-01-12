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
#include "network/logicerror.h"                 // LogicError
#include "network/sun-len-limits.h"             // sun_len_max,
                                                // sun_len_min
#include "network/sun-len-type.h"               // sun_len_type
#include "network/sunlengtherror.h"             // SunLengthError

#ifndef WIN32
#include <sys/socket.h>     // AF_UNIX
#include <sys/un.h>         // sockaddr_un
#endif

#include <string>       // std::to_string()
#include <utility>      // cmp_greater(), cmp_less()

#ifndef WIN32

auto Network::validate(const sockaddr_un* sun,
                       sun_len_type sun_len) -> void
{
    const auto size_max {sun_len_max};
    const auto size_min {sun_len_min};

    if (std::cmp_less(sun_len, size_min) ||
        std::cmp_greater(sun_len, size_max)) {
        throw SunLengthError(std::to_string(sun_len), size_max);
    }

    if (sun->sun_family != AF_UNIX) {
        throw LogicError("Invalid UNIX domain socket address");
    }
}

#endif
