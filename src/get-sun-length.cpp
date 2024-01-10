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

#include "network/get-sun-length.h"             // get_sun_length()
#include "network/get-path-length.h"            // get_path_length()
#include "network/sun-len-type.h"               // sun_len_type
#include "network/sun-offsets.h"                // sun_path_offset

#ifndef WIN32
#include <sys/un.h>         // sockaddr_un
#endif

#ifndef WIN32

auto Network::get_sun_length(const sockaddr_un* sun,
                             sun_len_type sun_len) -> sun_len_type
{
    const auto path_len {get_path_length(sun, sun_len)};
    const auto nul_len {path_len == 0U ? 0U : 1U};
    return sun_path_offset + path_len + nul_len;
}

#endif
