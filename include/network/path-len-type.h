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

#ifndef NETWORK_PATH_LEN_TYPE_H
#define NETWORK_PATH_LEN_TYPE_H

#include "network/get-sun-path-size.h"  // get_sun_path_size()

#include <cstddef>      // std::size_t

namespace Network
{
#ifndef WIN32
    using path_len_type = std::size_t;
    static constexpr auto path_len_max {get_sun_path_size() - 1};
    static constexpr auto path_len_min {0};
#endif
}

#endif