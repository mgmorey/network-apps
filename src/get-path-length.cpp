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

#include "network/get-path-length.h"            // get_path_length(),
                                                // sockaddr_un,
                                                // std::size_t
#include "network/get-path-pointer-sun.h"       // get_path_pointer()
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/to-sun-len.h"                 // sun_len_min,
                                                // to_sun_len()

#include <cstring>      // ::strnlen()

#ifndef WIN32

auto Network::get_path_length(const sockaddr_un* sun,
                              std::size_t size) -> std::size_t
{
#ifdef HAVE_SOCKADDR_SA_LEN
    const std::size_t sun_len {to_sun_len(sun->sun_len, size)};
#else
    const std::size_t sun_len {to_sun_len(size)};
#endif
    const auto path_len {sun_len - sun_len_min};
    const auto* path {get_path_pointer(sun)};
    return ::strnlen(path, path_len);
}

#endif
