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

#include "network/socketflags.h"        // SocketFlags, operator<<(),
                                        // std::ostream

#ifdef WIN32
#include <ws2tcpip.h>   // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME,
                        // AI_NUMERICHOST, AI_NUMERICSERV, AI_PASSIVE,
                        // AI_V4MAPPED
#else
#include <netdb.h>      // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME,
                        // AI_NUMERICHOST, AI_NUMERICSERV, AI_PASSIVE,
                        // AI_V4MAPPED
#endif

#include <sstream>      // std::ostringstream
#include <utility>      // std::pair
#include <vector>       // std::vector

auto Network::operator<<(std::ostream& os,
                         const SocketFlags& flags) -> std::ostream&
{
    static const std::vector<std::pair<int, const char*>> values {
        {AI_PASSIVE,        "AI_PASSIVE"},
        {AI_CANONNAME,      "AI_CANONNAME"},
        {AI_NUMERICHOST,    "AI_NUMERICHOST"},
        {AI_NUMERICSERV,    "AI_NUMERICSERV"},
        {AI_ALL,            "AI_ALL"},
        {AI_ADDRCONFIG,     "AI_ADDRCONFIG"},
        {AI_V4MAPPED,       "AI_V4MAPPED"},
    };

    std::ostringstream oss;
    std::size_t index {0};

    for(const auto& value : values) {
        if ((socket_flags_type {flags} & value.first) != 0) {
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
