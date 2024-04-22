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

#ifndef NETWORK_TO_SIZE_H
#define NETWORK_TO_SIZE_H

#include "network/sizeerror.h"          // SizeError

#include <cstddef>      // std::size_t
#include <string>       // std::to_string()
#include <utility>      // std::in_range()

namespace Network
{
    auto to_size(auto value) -> std::size_t
    {
        if (!std::in_range<std::size_t>(value)) {
            throw SizeError(std::to_string(value));
        }

        return static_cast<std::size_t>(value);
    }
}

#endif
