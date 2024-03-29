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

#ifndef NETWORK_TO_SA_LEN_H
#define NETWORK_TO_SA_LEN_H

#include "network/get-sa-size-maximum.h"        // get_sa_size_maximum()
#include "network/get-sa-size-minimum.h"        // get_sa_size_minimum()
#include "network/sa-len-limits.h"              // sa_len_max,
                                                // sa_len_min
#include "network/sa-len-type.h"                // sa_len_type
#include "network/salengtherror.h"              // SaLengthError
#include "network/socket-family-type.h"         // socket_family_type

#include <string>       // std::to_string()
#include <utility>      // std::cmp_greater(), std::cmp_less()

namespace Network
{
    auto to_sa_len(auto value,
                   sa_len_type size_min = sa_len_min,
                   sa_len_type size_max = sa_len_max) -> sa_len_type
    {
        if (std::cmp_less(value, size_min) ||
            std::cmp_greater(value, size_max)) {
            throw SaLengthError(std::to_string(value), size_min, size_max);
        }

        return static_cast<sa_len_type>(value);
    }

    auto to_sa_len(auto value, socket_family_type family) -> sa_len_type
    {
        const auto size_max {get_sa_size_maximum(family)};
        const auto size_min {get_sa_size_minimum(family)};
        return to_sa_len(value, size_min, size_max);
    }
}

#endif
