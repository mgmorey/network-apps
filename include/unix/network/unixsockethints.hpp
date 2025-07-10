// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef UNIX_NETWORK_UNIXSOCKETHINTS_HPP
#define UNIX_NETWORK_UNIXSOCKETHINTS_HPP

#ifndef _WIN32

#include "network/socket-family-enum.hpp"       // socket_family_enum
#include "network/socket-flags-type.hpp"        // socket_flags_type
#include "network/socket-type-type.hpp"         // socket_type_type
#include "network/sockethints.hpp"              // SocketHints

namespace Network
{
    struct UnixSocketHints : public SocketHints
    {
        constexpr explicit UnixSocketHints(socket_type_type t_socktype,
                                           socket_flags_type t_flags = 0) noexcept :
            SocketHints(af_unix, t_socktype, 0, t_flags)
        {
        }
    };
}

#endif

#endif
