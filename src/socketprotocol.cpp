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

#include "network/socketprotocol.h"             // SocketProtocol,
                                                // operator<<()

#ifdef WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // IPPROTO_ICMP, IPPROTO_IGMP, IPPROTO_IP,
                        // IPPROTO_RAW, IPPROTO_TCP, IPPROTO_UDP
#else
#include <netinet/in.h> // IPPROTO_ICMP, IPPROTO_IGMP, IPPROTO_IP,
                        // IPPROTO_RAW, IPPROTO_TCP, IPPROTO_UDP
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC
#endif

#include <ostream>      // std::ostream

auto Network::operator<<(std::ostream& os,
                         const SocketProtocol& protocol) noexcept ->
    std::ostream&
{
    switch (protocol.family()) {
    case AF_UNSPEC:
    case AF_INET:
    case AF_INET6:
        switch (protocol) {
        case IPPROTO_IP:
            os << "IPPROTO_IP";
            break;
        case IPPROTO_ICMP:
            os << "IPPROTO_ICMP";
            break;
        case IPPROTO_TCP:
            os << "IPPROTO_TCP";
            break;
        case IPPROTO_UDP:
            os << "IPPROTO_UDP";
            break;
        case IPPROTO_RAW:
            os << "IPPROTO_RAW";
            break;
        default:
            os << static_cast<int>(protocol);
        }
        break;
    default:
        os << static_cast<int>(protocol);
    }

    return os;
}
