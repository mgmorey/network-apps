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

#ifndef NETWORK_TO_NAME_LEN_H
#define NETWORK_TO_NAME_LEN_H

#include "network/logicerror.h"         // LogicError
#include "network/name-len-type.h"      // name_len_type

#include <string>       // std::string

namespace Network
{
    template<typename T>
    auto to_name_len(T value) -> name_len_type
    {
        if (value > name_len_max) {
            throw LogicError("Value " + std::to_string(value) +
                             " is too large to represent "
                             "as name_len_type");
        }

        return static_cast<name_len_type>(value);
    }
}

#endif
