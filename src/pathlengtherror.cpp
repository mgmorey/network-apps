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

#include "network/pathlengtherror.h"    // PathLengthError, RangeError
#include "network/path-len-type.h"      // path_len_max, path_len_min

#include <sstream>      // std::ostringstream

auto Network::PathLengthError::format(const std::string& t_value) -> std::string
{
    std::ostringstream oss;
    oss << "Value "
        << t_value
        << " is out of range ["
        << path_len_min
        << ", "
        << path_len_max
        << "] of path_len_type";
    return oss.str();
}

Network::PathLengthError::PathLengthError(const std::string& t_value) noexcept :
    RangeError(format(t_value))
{
}

Network::PathLengthError::PathLengthError(std::string&& t_value) noexcept :
    RangeError(format(t_value))
{
}