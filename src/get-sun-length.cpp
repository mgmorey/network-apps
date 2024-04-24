// Copyright (C) 2022  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/get-sun-length.h"             // get_sun_length()
#include "network/get-path-length.h"            // get_path_length()
#include "network/length-type.h"                // length_type
#include "network/path-length-limits.h"         // path_length_min
#include "network/sun-offsets.h"                // sun_path_offset

#include <sys/un.h>         // sockaddr_un

auto Network::get_sun_length(const sockaddr_un* sun,
                             length_type size) -> length_type
{
    const auto path_len {get_path_length(sun, size)};
    auto sun_len {sun_path_offset + path_len};

    if (path_len > path_length_min) {
        ++sun_len;
    }

    return sun_len;
}

#endif
