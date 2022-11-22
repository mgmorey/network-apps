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

#ifndef NETWORK_TO_INTEGER_H
#define NETWORK_TO_INTEGER_H

#include "network/byte.h"               // Byte, std::to_integer()
#include "network/integererror.h"       // IntegerError

#include <climits>      // INT_MAX, INT_MIN
#include <string>       // std::to_string()
#include <utility>      // cmp_greater(), cmp_less()

namespace Network
{
    template<typename T>
    constexpr auto to_integer(Byte byte) noexcept -> T
    {
        return std::to_integer<T>(byte);
    }

    constexpr auto to_integer(auto value) -> int
    {
        if (std::cmp_less(value, INT_MIN) ||
            std::cmp_greater(value, INT_MAX)) {
            throw IntegerError(std::to_string(value));
        }

        return static_cast<int>(value);
    }
}

#endif
