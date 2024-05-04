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

#ifndef NETWORK_SOCKET_IP_PROTOCOL_ENUM_H
#define NETWORK_SOCKET_IP_PROTOCOL_ENUM_H

#ifdef WIN32
#include <winsock2.h>       // IPPROTO_IP, IPPROTO_RAW, IPPROTO_TCP,
                            // IPPROTO_UDP
#else
#include <netinet/in.h>     // IPPROTO_IP, IPPROTO_RAW, IPPROTO_TCP,
                            // IPPROTO_UDP
#endif

#include <cstdint>      // std::uint8_t

namespace Network
{
    using socket_ip_protocol_enum = enum : std::uint8_t {
        ipproto_ip = IPPROTO_IP,
        ipproto_tcp = IPPROTO_TCP,
        ipproto_udp = IPPROTO_UDP,
        ipproto_raw = IPPROTO_RAW
    };
}

#endif
