// Copyright (C) 2023  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_LENGTHERROR_H
#define NETWORK_LENGTHERROR_H

#include "network/rangeerror.h"         // RangeError

#include <cstddef>      // std::size_t
#include <string>       // std::string

namespace Network
{
    class LengthError :
        public RangeError
    {
    public:
        explicit LengthError(const std::string& t_value,
                             std::size_t t_minimum,
                             std::size_t t_maximum,
                             const std::string& t_type) noexcept;
        LengthError(const LengthError&) noexcept = default;
        LengthError(LengthError&&) noexcept = default;
        ~LengthError() noexcept override = default;
        auto operator=(const LengthError&) noexcept ->
            LengthError& = default;
        auto operator=(LengthError&&) noexcept ->
            LengthError& = default;
    };
}

#endif
