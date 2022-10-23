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

#include "network/path-len-type.h"      // path_len_type
#include "network/pathlengtherror.h"    // PathLengthError

#include <string>       // std::to_string()

namespace Network
{
#ifndef WIN32
    template<typename T>
    auto to_path_len(T value) -> path_len_type
    {
        if (value < path_len_min ||
            value > path_len_max) {
            throw PathLengthError(std::to_string(value));
        }

        return static_cast<path_len_type>(value);
    }
#endif
}

#endif
