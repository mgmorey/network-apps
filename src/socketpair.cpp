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
#include "network/create-pair.h"                // create_pair()
#include "network/sharedsocket.h"               // SharedSocket
#include "network/sockethints.h"                // SocketHints

#include <utility>      // std::move()

Network::SocketPair::SocketPair(const SocketHints& t_hints, bool t_is_verbose) :
    SocketPair(create_pair(t_hints, t_is_verbose))
{
}

Network::SocketPair::SocketPair(SharedSocket t_socket_1,
                                SharedSocket t_socket_2) noexcept :
    m_socket_pair({std::move(t_socket_1), std::move(t_socket_2)})
{
}

auto Network::SocketPair::at(size_type t_index) const -> SharedSocket
{
    return m_socket_pair.at(t_index);
}

auto Network::SocketPair::at(size_type t_index) -> SharedSocket
{
    return m_socket_pair.at(t_index);
}

#endif
