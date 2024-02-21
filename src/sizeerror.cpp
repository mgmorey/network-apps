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

#include "network/sizeerror.h"          // SizeError
#include "network/rangeerror.h"         // RangeError

#include <cstdint>      // SIZE_MAX
#include <string>       // std::string, std::to_string()

Network::SizeError::SizeError(const std::string& t_value) noexcept :
    RangeError(t_value,
               std::to_string(0),
               std::to_string(SIZE_MAX),
               "std::size_t")
{
}
