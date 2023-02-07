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

#include "network/sa-len-limits.h"      // sa_len_max, sa_len_min
#include "network/sa-len-type.h"        // sa_len_type
#include "network/salengtherror.h"      // SaLengthError

#include <string>       // std::to_string()
#include <utility>      // cmp_greater(), cmp_less()

namespace Network
{
    auto to_sa_len(auto value) -> sa_len_type
    {
        if (std::cmp_less(value, sa_len_min) ||
            std::cmp_greater(value, sa_len_max)) {
            throw SaLengthError(std::to_string(value));
        }

        return static_cast<sa_len_type>(value);
    }
}

#endif
