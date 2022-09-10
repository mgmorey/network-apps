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

#ifndef NETWORK_TO_SIZE_H
#define NETWORK_TO_SIZE_H

#include "network/rangeerror.h"         // RangeError

#include <climits>      // SIZE_MAX
#include <cstddef>      // std::size_t
#include <string>       // std::string

namespace Network
{
    template<typename T>
    auto to_size(T value) -> std::size_t
    {
        if (value < 0) {
            throw RangeError("Value " + std::to_string(value) +
                             " is out of range [" +
                             std::to_string(0) +
                             ", " +
                             std::to_string(SIZE_MAX) +
                             "] of std::size_t");
        }

        return static_cast<std::size_t>(value);
    }
}

#endif
