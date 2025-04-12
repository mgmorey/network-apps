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

#ifndef NETWORK_IPSOCKETHINTS_H
#define NETWORK_IPSOCKETHINTS_H

#include "network/socket-flags-type.hpp"        // socket_flags_type
#include "network/socket-ip-family-enum.hpp"    // socket_ip_family_enum
#include "network/socket-ip-protocol-enum.hpp"  // socket_ip_protocol_enum
#include "network/socket-type-type.hpp"         // socket_type_type
#include "network/sockethints.hpp"              // SocketHints

namespace Network
{
    struct IpSocketHints : public SocketHints
    {
        constexpr IpSocketHints(socket_ip_family_enum t_family,
                                socket_type_type t_socktype,
                                socket_ip_protocol_enum t_protocol,
                                socket_flags_type t_flags = 0) noexcept :
            SocketHints(t_family, t_socktype, t_protocol, t_flags)
        {
        }

        constexpr IpSocketHints(socket_ip_family_enum t_family,
                                socket_type_type t_socktype,
                                socket_flags_type t_flags = 0) noexcept :
            SocketHints(t_family, t_socktype, 0, t_flags)
        {
        }

        constexpr IpSocketHints(socket_ip_family_enum t_family,
                                socket_ip_protocol_enum t_protocol,
                                socket_flags_type t_flags = 0) noexcept :
            SocketHints(t_family, 0, t_protocol, t_flags)
        {
        }

        constexpr explicit IpSocketHints(socket_type_type t_socktype,
                                         socket_flags_type t_flags = 0) noexcept :
            SocketHints(0, t_socktype, 0, t_flags)
        {
        }

        constexpr explicit IpSocketHints(socket_ip_protocol_enum t_protocol,
                                         socket_flags_type t_flags = 0) noexcept :
            SocketHints(0, 0, t_protocol, t_flags)
        {
        }
    };
}

#endif
