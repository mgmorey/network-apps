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

#ifndef WIN32

#include "network/socketpair.h"                 // SocketPair
#include "network/create-socketpair.h"          // create_socketpair()
#include "network/socket.h"                     // Socket
#include "network/sockethints.h"                // SocketHints

#include <cstddef>      // std::size_t

Network::SocketPair::SocketPair(const SocketHints& t_hints, bool t_verbose) :
    SocketPair(create_socketpair(t_hints, t_verbose))
{
}

Network::SocketPair::SocketPair(const Socket& t_socket1,
                                const Socket& t_socket2) noexcept :
    m_socket({t_socket1, t_socket2})
{
}

auto Network::SocketPair::at(std::size_t index) const -> const Socket&
{
    return m_socket.at(index);
}

auto Network::SocketPair::at(std::size_t index) -> Socket&
{
    return m_socket.at(index);
}

#endif
