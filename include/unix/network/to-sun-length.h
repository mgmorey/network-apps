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

#ifndef UNIX_NETWORK_TO_SUN_LENGTH_H
#define UNIX_NETWORK_TO_SUN_LENGTH_H

#ifndef WIN32

#include "network/length-type.h"                // length_type
#include "network/sun-length-limits.h"          // sun_length_max,
                                                // sun_length_min
#include "network/to-value.h"                   // to_value()

namespace Network
{
    auto to_sun_length(auto value, length_type size_max) -> length_type
    {
        const length_type size_min {sun_length_min};

        return to_value<length_type>("sun_length_type", value,
                                     size_min,
                                     size_max);
    }

    auto to_sun_length(auto value) -> length_type
    {
        return to_sun_length(value, sun_length_max);
    }
}

#endif

#endif
