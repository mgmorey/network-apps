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

#ifndef NETWORK_TO_PATH_LEN_H
#define NETWORK_TO_PATH_LEN_H

#include "network/path-length-limits.h"         // path_length_max,
                                                // path_length_min
#include "network/path-length-type.h"           // path_length_type
#include "network/pathlengtherror.h"            // PathLengthError

#include <string>       // std::to_string()
#include <utility>      // std::cmp_greater(), std::cmp_less()

namespace Network
{
#ifndef WIN32
    auto to_path_length(auto value) -> path_length_type
    {
        if (std::cmp_less(value, path_length_min) ||
            std::cmp_greater(value, path_length_max)) {
            throw PathLengthError(std::to_string(value));
        }

        return static_cast<path_length_type>(value);
    }
#endif
}

#endif
