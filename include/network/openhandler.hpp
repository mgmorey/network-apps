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

#ifndef NETWORK_OPENHANDLER_HPP
#define NETWORK_OPENHANDLER_HPP

#include "network/handle-type.hpp"      // handle_type
#include "network/symbol.hpp"           // Symbol

#ifdef WIN32
#include <winsock2.h>       // sockaddr
#include <ws2tcpip.h>       // socklen_t
#else
#include <sys/socket.h>     // sockaddr, socklen_t
#endif

#include <string_view>  // std::string_view
#include <tuple>        // std::tuple

namespace Network
{
    using OpenFunction = int (*)(handle_type, const sockaddr*, socklen_t);
    using OpenHandler = std::tuple<OpenFunction, std::string_view, Symbol>;
}

#endif
