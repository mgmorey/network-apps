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

#ifndef NETWORK_TO_NAME_LENGTH_H
#define NETWORK_TO_NAME_LENGTH_H

#include "network/name-length-limits.h"         // name_length_max,
                                                // name_length_min
#include "network/name-length-type.h"           // name_length_type
#include "network/namelengtherror.h"            // NameLengthError

#include <string>       // std::to_string()
#include <utility>      // std::cmp_greater(), std::cmp_less()

namespace Network
{
    auto to_name_length(auto value) -> name_length_type
    {
        if (std::cmp_less(value, name_length_min) ||
            std::cmp_greater(value, name_length_max)) {
            throw NameLengthError(std::to_string(value));
        }

        return static_cast<name_length_type>(value);
    }
}

#endif
