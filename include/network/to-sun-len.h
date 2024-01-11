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

#ifndef NETWORK_TO_SUN_LEN_H
#define NETWORK_TO_SUN_LEN_H

#ifndef WIN32

#include "network/sun-len-limits.h"     // sun_len_max, sun_len_min
#include "network/sun-len-type.h"       // sun_len_type
#include "network/sunlengtherror.h"     // SunLengthError

#include <string>       // std::to_string()
#include <utility>      // cmp_greater(), cmp_less()

namespace Network
{
    auto to_sun_len(auto value,
                    sun_len_type size_max) -> sun_len_type
    {
        const sun_len_type size_min {sun_len_min};

        if (std::cmp_less(value, size_min) ||
            std::cmp_greater(value, size_max)) {
            throw SunLengthError(std::to_string(value), size_max);
        }

        return static_cast<sun_len_type>(value);
    }

    auto to_sun_len(auto value) -> sun_len_type
    {
        return to_sun_len(value, sun_len_max);

    }
}

#endif
#endif
