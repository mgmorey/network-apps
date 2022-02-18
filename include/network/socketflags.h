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

#ifndef NETWORK_FLAGS_H
#define NETWORK_FLAGS_H

#include "network/integer.h"            // Integer
#include "network/socket-flags-type.h"  // socket_flags_type

#include <ostream>      // std::ostream

namespace Network
{
    class SocketFlags :
        public Integer<socket_flags_type>
    {
    public:
        constexpr explicit SocketFlags(socket_flags_type t_value) noexcept :
            Integer(t_value)
        {
        }
    };

    extern auto operator<<(std::ostream& os,
                           const SocketFlags& flags) noexcept -> std::ostream&;
}

#endif
