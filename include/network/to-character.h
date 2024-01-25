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

#ifndef NETWORK_TO_CHARACTER_H
#define NETWORK_TO_CHARACTER_H

#include "network/charactererror.h"     // CharacterError

#include <climits>      // CHAR_MAX, CHAR_MIN
#include <string>       // std::to_string()
#include <utility>      // std::cmp_greater(), std::cmp_less()

namespace Network
{
    auto to_character(auto value) -> char
    {
        if (std::cmp_less(value, CHAR_MIN) ||
            std::cmp_greater(value, CHAR_MAX)) {
            throw CharacterError(std::to_string(value));
        }

        return static_cast<char>(value);
    }
}

#endif
