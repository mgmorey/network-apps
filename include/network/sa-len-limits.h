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

#ifndef NETWORK_SA_LEN_LIMITS_H
#define NETWORK_SA_LEN_LIMITS_H

#include "network/sa-offsets.h"         // sa_data_offset
#include "network/sizes.h"              // sa_size_max

namespace Network
{
    static constexpr int sa_len_max {sa_size_max};
    static constexpr int sa_len_min {sa_data_offset};
}

#endif
