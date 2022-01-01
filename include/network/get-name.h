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

#ifndef NETWORK_GET_NAME_H
#define NETWORK_GET_NAME_H

#include "network/bytes.h"              // Bytes
#include "network/fd.h"                 // Fd, fd_type
#include "network/result.h"             // Result

#ifdef _WIN32
#include <winsock2.h>       // sockaddr, socklen_t
#include <ws2tcpip.h>       // socklen_t
#else
#include <netdb.h>          // socklen_t
#include <sys/socket.h>     // sockaddr, socklen_t
#endif

#include <string>       // std::string
#include <utility>      // std::pair
#include <variant>      // std::variant

namespace Network
{
    using BytesResult = std::variant<Bytes, Result>;
    using GetNameFunction = int (*)(fd_type, sockaddr*, socklen_t*);
    using GetNameHandler = std::pair<GetNameFunction, const char*>;

    extern auto get_name(const GetNameHandler& handler, Fd fd,
                         bool verbose) -> BytesResult;
}

#endif
