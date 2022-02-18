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

#include "network/socktype.h"           // SockType, operator<<(),
                                        // std::ostream

#ifdef WIN32
#include <winsock2.h>   // SOCK_DGRAM, SOCK_RAW, SOCK_RDM,
                        // SOCK_SEQPACKET, SOCK_STREAM
#else
#include <sys/socket.h> // SOCK_DGRAM, SOCK_DCCP, SOCK_PACKET,
                        // SOCK_RAW, SOCK_RDM, SOCK_SEQPACKET,
                        // SOCK_STREAM
#endif

#include <sstream>      // std::ostringstream
#include <vector>       // std::vector

auto Network::operator<<(std::ostream& os,
                         const SockType& socktype) noexcept -> std::ostream&
{
    static const std::vector<std::pair<int, const char*>> values {
#ifdef SOCK_CLOEXEC
        {SOCK_CLOEXEC,      "SOCK_CLOEXEC"},
#endif
#ifdef SOCK_NONBLOCK
        {SOCK_NONBLOCK,     "SOCK_NONBLOCK"},
#endif
        {0,             nullptr}
    };

    constexpr auto mask {0000017};
    std::ostringstream oss;
    std::size_t i {0};

    if ((static_cast<socktype_type>(socktype) & mask) != 0) {
        switch (socktype & mask) {
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
            oss << static_cast<socktype_type>(socktype);
        }
        ++i;
    }

    for (const auto& value : values) {
        if ((socktype & value.first) != 0) {
            if (i++ > 0) {
                oss << " | ";
            }

            oss << value.second;
        }
    }

    switch (i) {
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
