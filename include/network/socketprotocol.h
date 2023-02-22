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

#ifndef NETWORK_SOCKETPROTOCOL_H
#define NETWORK_SOCKETPROTOCOL_H

#include "network/integer.h"                    // Integer
#include "network/socketfamily.h"               // SocketFamily,
                                                // socket_family_type
#include "network/socket-protocol-type.h"       // socket_protocol_type

#include <ostream>      // std::ostream

namespace Network
{
    class SocketProtocol :
        public Integer<socket_protocol_type>
    {
        friend auto operator<<(std::ostream& os,
                               const SocketProtocol& protocol) -> std::ostream&;

    public:
        constexpr SocketProtocol(socket_family_type t_family,
                                 socket_protocol_type t_value) noexcept :
            Integer(t_value),
            m_family(t_family)
        {
        }

        [[nodiscard]] constexpr auto family() const noexcept -> SocketFamily
        {
            return m_family;
        }

    private:
        SocketFamily m_family;
    };

    extern auto operator<<(std::ostream& os,
                           const SocketProtocol& protocol) -> std::ostream&;
}

#endif
