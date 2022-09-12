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

#include "network/namelengtherror.h"    // NameLengthError, RangeError
#include "network/name-len-type.h"      // name_len_max, name_len_min

auto Network::NameLengthError::format(const std::string& t_value) -> std::string
{
    return ("Value " + t_value + " is out of range [" +
            std::to_string(name_len_min) + ", " +
            std::to_string(name_len_max) +
            "] of name_len_type");
}

Network::NameLengthError::NameLengthError(const std::string& t_value) noexcept :
    RangeError(format(t_value))
{
}

Network::NameLengthError::NameLengthError(std::string&& t_value) noexcept :
    RangeError(format(t_value))
{
}
