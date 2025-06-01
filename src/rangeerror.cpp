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

#include "network/rangeerror.hpp"       // RangeError
#include "network/logicerror.hpp"       // LogicError

#include <format>       // std::format()
#include <string_view>  // std::string_view

Network::RangeError::RangeError(std::string_view t_value,
                                std::string_view t_min,
                                std::string_view t_max,
                                std::string_view t_type) noexcept :
    LogicError(std::format("Value {} is out of range [{}, {}] of {}",
                           t_value, t_min, t_max, t_type))
{
}
