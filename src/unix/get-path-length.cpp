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

#ifndef _WIN32

#include "network/get-path-length.hpp"  // get_path_length()
#include "network/length-type.hpp"      // length_type
#include "network/sun-offsets.hpp"      // sun_path_offset

#include <sys/un.h>         // sockaddr_un

#include <cstring>      // ::strnlen()

auto Network::get_path_length(const sockaddr_un* sun,
                              length_type sun_len) -> length_type
{
    if (sun_len <= sun_path_offset) {
        return 0UL;
    }

    const auto* const path {static_cast<const char*>(sun->sun_path)};
    const auto path_len {sun_len - sun_path_offset};
    return ::strnlen(path, path_len - 1);
}

#endif
