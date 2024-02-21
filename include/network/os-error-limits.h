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

#ifndef NETWORK_OS_ERROR_LIMITS_H
#define NETWORK_OS_ERROR_LIMITS_H

#include <climits>      // INT_MAX, INT_MIN
#include <cstdint>      // UINT32_MAX

namespace Network
{
#ifdef WIN32
    static constexpr auto os_error_max {UINT32_MAX};
    static constexpr auto os_error_min {0};
#else
    static constexpr auto os_error_max {INT_MAX};
    static constexpr auto os_error_min {INT_MIN};
#endif
}

#endif
