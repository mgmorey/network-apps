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

#ifndef NETWORK_TO_PATH_LENGTH_H
#define NETWORK_TO_PATH_LENGTH_H

#ifndef WIN32

#include "network/length-type.h"                // length_type
#include "network/path-length-limits.h"         // path_length_max,
                                                // path_length_min
#include "network/to-value.h"                   // to_value()

namespace Network
{
    auto to_path_length(auto value) -> length_type
    {
        return to_value<length_type>("path_length_type", value,
                                     path_length_min,
                                     path_length_max);
    }
}

#endif

#endif
