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

#include "network/get-path-length.h"            // get_path_length()
#include "network/length-type.h"                // length_type
#include "network/sun-length-limits.h"          // sun_length_min

#include <sys/un.h>         // sockaddr_un

#include <cstring>      // ::strnlen()

auto Network::get_path_length(const char* path,
                              length_type path_len) -> length_type
{
    if (path_len == 0UL) {
        return path_len;
    }

    return ::strnlen(path, path_len - 1);
}

auto Network::get_path_length(const sockaddr_un* sun,
                              length_type sun_len) -> length_type
{
    const auto* const path {static_cast<const char*>(sun->sun_path)};
    const auto path_len {sun_len - sun_length_min};
    return get_path_length(path, path_len);
}

#endif
