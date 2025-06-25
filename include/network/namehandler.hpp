// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_NAMEHANDLER_HPP
#define NETWORK_NAMEHANDLER_HPP

#include "network/handler.hpp"          // Handler
#include "network/handle-type.hpp"      // handle_type

#ifdef WIN32
#include <winsock2.h>       // sockaddr
#include <ws2tcpip.h>       // socklen_t
#else
#include <sys/socket.h>     // sockaddr, socklen_t
#endif

namespace Network
{
    using NameFunction = int (*)(handle_type, sockaddr*, socklen_t*);
    using NameHandler = Handler<NameFunction>;
}

#endif
