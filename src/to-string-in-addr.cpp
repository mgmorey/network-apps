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

#include "network/to-string-in-addr.hpp"        // to_string()
#include "network/textbuffer.hpp"               // TextBuffer

#ifdef _WIN32
#include <winsock2.h>       // AF_INET, in_addr
#include <ws2tcpip.h>       // INET_ADDRSTRLEN, inet_ntop()
#else
#include <arpa/inet.h>      // inet_ntop()
#include <netinet/in.h>     // in_addr
#include <sys/socket.h>     // AF_INET, INET_ADDRSTRLEN
#endif

#include <string>       // std::string

auto Network::to_string(const in_addr& addr) noexcept -> std::string
{
    TextBuffer buffer {INET_ADDRSTRLEN};
    ::inet_ntop(AF_INET, &addr, buffer.data(), buffer.size());
    return buffer;
}
