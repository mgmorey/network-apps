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

#include "network/to-string-in6-addr.h"         // in6_addr,
                                                // std::string(),
                                                // to_string()
#include "network/buffer.h"                     // Buffer

#ifdef WIN32
#include <winsock2.h>       // AF_INET6
#include <ws2tcpip.h>       // INET6_ADDRSTRLEN, inet_ntop()
#else
#include <arpa/inet.h>      // inet_ntop()
#include <sys/socket.h>     // AF_INET6, INET6_ADDRSTRLEN
#endif

auto Network::to_string(const in6_addr& addr) noexcept -> std::string
{
    Buffer buffer {INET6_ADDRSTRLEN};
    ::inet_ntop(AF_INET6, &addr, buffer.data(), buffer.size());
    return std::string {buffer};
}
