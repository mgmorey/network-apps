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

#ifndef NETWORK_TO_SOCKET_LENGTH_HPP
#define NETWORK_TO_SOCKET_LENGTH_HPP

#include "network/sa-length-limits.hpp"         // sa_length_max,
                                                // sa_length_min
#include "network/socket-length-type.hpp"       // socket_length_type
#include "network/to-value.hpp"                 // to_value()

namespace Network
{
    auto to_socket_length(auto value) -> socket_length_type
    {
        return to_value<socket_length_type>("socket_length_type", value,
                                            sa_length_min,
                                            sa_length_max);
    }
}

#endif
