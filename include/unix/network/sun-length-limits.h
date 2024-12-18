// Copyright (C) 2023  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef UNIX_NETWORK_SUN_LENGTH_LIMITS_H
#define UNIX_NETWORK_SUN_LENGTH_LIMITS_H

#ifndef WIN32

#include "network/sun-offsets.h"                // sun_path_offset
#include "network/sun-sizes.h"                  // sun_size

namespace Network
{
    static constexpr auto sun_length_max {sun_size};
    static constexpr auto sun_length_min {sun_path_offset};
}

#endif

#endif
