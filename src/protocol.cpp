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

#include "network/protocol.h"           // Protocol, operator<<(),
                                        // std::ostream
#include "network/os-features.h"        // WIN32

#ifdef WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // IPPROTO_ICMP, IPPROTO_IGMP, IPPROTO_IP,
                        // IPPROTO_RAW, IPPROTO_TCP, IPPROTO_UDP
#else
#include <netinet/in.h> // IPPROTO_ICMP, IPPROTO_IGMP, IPPROTO_IP,
                        // IPPROTO_RAW, IPPROTO_TCP, IPPROTO_UDP
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
#endif

Network::Protocol::Protocol(family_type t_family, protocol_type t_value) :
    Integer(t_value),
    m_family(t_family)
{
}

auto Network::Protocol::family() const -> Network::Family
{
    return m_family;
}

auto Network::operator<<(std::ostream& os,
                         const Protocol& protocol) -> std::ostream&
{
    const auto family {static_cast<int>(protocol.family())};

    switch (family) {
    case AF_INET:
    case AF_INET6:
    case AF_UNSPEC:
        switch (protocol) {
        case IPPROTO_IP:
            os << "IPPROTO_IP";
            break;
        case IPPROTO_ICMP:
            os << "IPPROTO_ICMP";
            break;
        case IPPROTO_IGMP:
            os << "IPPROTO_IGMP";
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
            os << static_cast<protocol_type>(protocol);
        }
        break;
    default:
        os << static_cast<protocol_type>(protocol);
    }

    return os;
}
