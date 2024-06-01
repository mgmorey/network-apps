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

#ifndef NETWORK_HOSTNAME_LENGTH_LIMITS_H
#define NETWORK_HOSTNAME_LENGTH_LIMITS_H

#include "network/name-length-limits.h"         // name_length_max,
                                                // name_length_min

namespace Network
{
    static constexpr auto hostname_length_max {name_length_max};
    static constexpr auto hostname_length_min {name_length_min};
}

#endif