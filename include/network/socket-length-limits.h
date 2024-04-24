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

#ifndef NETWORK_SOCKET_LENGTH_LIMITS_H
#define NETWORK_SOCKET_LENGTH_LIMITS_H

#include "network/sa-len-limits.h"      // sa_len_max, sa_len_min

namespace Network
{
    static constexpr auto socket_length_max {sa_len_max};
    static constexpr auto socket_length_min {sa_len_min};
}

#endif
