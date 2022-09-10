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

#include "network/logicerror.h"         // LogicError

#include <climits>      // SIZE_MAX
#include <cstddef>      // std::size_t
#include <string>       // std::string

namespace Network
{
    template<typename T>
    auto to_size(T value) -> std::size_t
    {
        static constexpr auto size_max {SSIZE_MAX};
        static constexpr auto size_min {0};

        if (size_min > value || value > size_max) {
            throw LogicError("Value " + std::to_string(value) +
                             " is out of range [" +
                             std::to_string(size_min) +
                             ", " +
                             std::to_string(size_max) +
                             "] of std::size_t");
        }

        return static_cast<std::size_t>(value);
    }
}

#endif
