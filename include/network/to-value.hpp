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

#ifndef NETWORK_TO_VALUE_HPP
#define NETWORK_TO_VALUE_HPP

#include "network/valueerror.hpp"       // ValueError

#include <type_traits>  // std::is_arithmetic_v
#include <utility>      // std::cmp_greater(), std::cmp_less(),
                        // std::in_range()

namespace Network
{
    template <typename T, typename V>
    requires std::is_arithmetic_v<T>
    and std::is_arithmetic_v<V>
    auto to_value(const std::string& value_type, V value) -> T
    {
        if (!std::in_range<T>(value)) {
            throw ValueError<T> {value_type, value};
        }

        return static_cast<T>(value);
    }

    template <typename T, typename V>
    requires std::is_arithmetic_v<T>
    and std::is_arithmetic_v<V>
    auto to_value(const std::string& value_type, V value, T min, T max) -> T
    {
        if (std::cmp_less(value, min) || std::cmp_greater(value, max)) {
            throw ValueError<T> {value_type, value, min, max};
        }

        return static_cast<T>(value);
    }
}

#endif
