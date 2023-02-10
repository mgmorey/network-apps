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

#include "network/get-sun-length.h"             // get_sun_length(),
                                                // sockaddr_un,
                                                // sun_len_type
#include "network/get-path-length.h"            // get_path_length()

#ifndef WIN32

auto Network::get_sun_length(const sockaddr_un* sun,
                             sun_len_type size) noexcept -> sun_len_type
{
    const auto path_len {get_path_length(sun, size)};
    return sizeof *sun - sizeof sun->sun_path + path_len;
}

#endif
