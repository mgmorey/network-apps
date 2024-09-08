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

#ifndef NETWORK_VALUEERROR_H
#define NETWORK_VALUEERROR_H

#include "network/rangeerror.h"        // RangeError

#include <limits>       // std::numeric_limits
#include <string_view>  // std::string_view

namespace Network
{
    template <typename T>
    class ValueError :
        public RangeError
    {
    public:
        ValueError(const std::string_view& t_value_type,
                   auto t_value,
                   T t_min = std::numeric_limits<T>::min(),
                   T t_max = std::numeric_limits<T>::max()) noexcept :
            RangeError(std::to_string(t_value),
                       std::to_string(t_min),
                       std::to_string(t_max), t_value_type)
        {
        }

        ValueError(const ValueError&) noexcept = default;
        ValueError(ValueError&&) noexcept = default;
        ~ValueError() noexcept override = default;
        auto operator=(const ValueError&) noexcept -> ValueError& = default;
        auto operator=(ValueError&&) noexcept -> ValueError& = default;
    };
}

#endif
