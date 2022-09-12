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

#ifndef NETWORK_TO_SOCK_LEN_H
#define NETWORK_TO_SOCK_LEN_H

#include "network/socketlengtherror.h"  // SocketLengthError
#include "network/sock-len-type.h"      // sock_len_type

#include <string>       // std::to_string()

namespace Network
{
    template<typename T>
    auto to_sock_len(T value) -> sock_len_type
    {
        if (value < sock_len_min ||
            value > sock_len_max) {
            throw SocketLengthError(std::to_string(value));
        }

        return static_cast<sock_len_type>(value);
    }
}

#endif
