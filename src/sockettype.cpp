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

#include "network/sockettype.h"                 // SocketType
#include "network/socket-type-type.h"           // socket_type_type

#ifdef WIN32
#include <winsock2.h>   // SOCK_DGRAM, SOCK_RAW, SOCK_RDM,
                        // SOCK_SEQPACKET, SOCK_STREAM
#else
#include <sys/socket.h> // SOCK_DGRAM, SOCK_DCCP, SOCK_PACKET,
                        // SOCK_RAW, SOCK_RDM, SOCK_SEQPACKET,
                        // SOCK_STREAM
#endif

#include <cstddef>      // std::size_t
#include <ostream>      // std::ostream
#include <sstream>      // std::ostringstream
#include <utility>      // std::pair
#include <vector>       // std::vector

auto Network::operator<<(std::ostream& os,
                         const SocketType& type) noexcept -> std::ostream&
{
    static const std::vector<std::pair<int, const char*>> values {
#ifdef SOCK_CLOEXEC
        {SOCK_CLOEXEC,      "SOCK_CLOEXEC"},
#endif
#ifdef SOCK_NONBLOCK
        {SOCK_NONBLOCK,     "SOCK_NONBLOCK"},
#endif
        {0,                 nullptr}
    };
    static constexpr auto mask {0000017};

    const socket_type_type type_value {type};
    std::ostringstream oss;
    std::size_t index {0};

    if ((type & mask) != 0) {
        switch (type_value & mask) {
        case SOCK_STREAM:
            oss << "SOCK_STREAM";
            break;
        case SOCK_DGRAM:
            oss << "SOCK_DGRAM";
            break;
        case SOCK_RAW:
            oss << "SOCK_RAW";
            break;
        case SOCK_SEQPACKET:
            oss << "SOCK_SEQPACKET";
            break;
        default:
            oss << type_value;
        }

        ++index;
    }

    for (const auto& value : values) {
        if ((type & value.first) != 0) {
            if (index++ > 0) {
                oss << " | ";
            }

            oss << value.second;
        }
    }

    switch (index) {
    case 0:
        os << 0;
        break;
    case 1:
        os << oss.str();
        break;
    default:
        os << '(' << oss.str() << ')';
    }

    return os;
}
