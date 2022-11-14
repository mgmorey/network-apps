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

#include "network/socketpair.h"         // Socket, SocketPair
#include "network/get-socketpair.h"     // get_socketpair()

#include <cstddef>      // std::size_t

#ifndef WIN32

Network::SocketPair::SocketPair(socket_family_type t_family,
                        socket_type_type t_socktype,
                        socket_protocol_type t_protocol,
                        socket_flags_type t_flags,
                        bool t_verbose) :
    SocketPair(SocketHints {t_flags, t_family, t_socktype, t_protocol}, t_verbose)
{
}

Network::SocketPair::SocketPair(const SocketHints& t_hints, bool t_verbose) :
    SocketPair(get_socketpair(t_hints, t_verbose))
{
}

#endif

Network::SocketPair::SocketPair(const Socket& t_socket1,
                                const Socket& t_socket2) noexcept :
    m_socket({t_socket1, t_socket2})
{
}

auto Network::SocketPair::at(std::size_t index) const -> const Network::Socket&
{
    return m_socket.at(index);
}

auto Network::SocketPair::at(std::size_t index) -> Network::Socket&
{
    return m_socket.at(index);
}
