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

#include "network/integererror.h"       // IntegerError, RangeError

#include <climits>      // INT_MAX, INT_MIN
#if __has_include(<format>)
#include <format>       // std::format()
#else
#include <sstream>      // std::ostringstream
#endif

auto Network::IntegerError::format(const std::string& t_value) -> std::string
{
#ifdef __cpp_lib_format
    return std::format("Value {} is out of range [{}, {}] of int",
                       t_value, INT_MIN, INT_MAX);
#else
    std::ostringstream oss;
    oss << "Value "
        << t_value
        << " is out of range ["
        << INT_MIN
        << ", "
        << INT_MAX
        << "] of int";
    return oss.str();
#endif
}

Network::IntegerError::IntegerError(const std::string& t_value) noexcept :
    RangeError(format(t_value))
{
}

Network::IntegerError::IntegerError(std::string&& t_value) noexcept :
    RangeError(format(t_value))
{
}
