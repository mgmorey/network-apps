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

#include "network/get-path-length.h"            // get_path_length()
#include "network/get-path-pointer-sun.h"       // get_path_pointer()
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/path-len-type.h"              // path_len_type,
#include "network/sun-len-limits.h"             // sun_len_min
#include "network/to-sun-len.h"                 // to_sun_len()

#ifndef WIN32
#include <sys/un.h>         // sockaddr_un
#endif

#include <cstring>      // ::strnlen()

#ifndef WIN32

auto Network::get_path_length(const sockaddr_un* sun,
                              path_len_type size) -> path_len_type
{
#ifdef HAVE_SOCKADDR_SA_LEN
    const auto sun_len {to_sun_len(sun->sun_len, size)};
#else
    const auto sun_len {size};
#endif
    const auto* const path {get_path_pointer(sun)};
    const auto path_len {sun_len - sun_len_min};
    return ::strnlen(path, path_len);  // NOLINT
}

#endif
