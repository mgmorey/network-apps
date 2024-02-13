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

#include "network/namelengtherror.h"            // NameLengthError
#include "network/name-length-limits.h"         // name_length_max,
                                                // name_length_min
#include "network/rangeerror.h"                 // RangeError

#include <string>       // std::string
#include <utility>      // std::move()
#include <version>

#ifdef __cpp_lib_format
#include <format>       // std::format()
#else
#include <sstream>      // std::ostringstream
#endif

auto Network::NameLengthError::format(const std::string& t_str) -> std::string
{
#ifdef __cpp_lib_format
    return std::format("Value {} is out of range [{}, {}] of name_length_type",
                       t_str, name_length_min, name_length_max);
#else
    std::ostringstream oss;
    oss << "Value "
        << t_str
        << " is out of range ["
        << name_length_min
        << ", "
        << name_length_max
        << "] of name_length_type";
    return oss.str();
#endif
}

Network::NameLengthError::NameLengthError(const std::string& t_str) noexcept :
    RangeError(format(t_str))
{
}

Network::NameLengthError::NameLengthError(std::string&& t_str) noexcept :
    RangeError(format(t_str))
{
    static_cast<void>(std::move(t_str));
}
