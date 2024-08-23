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

#include "network/socket.h"                     // Socket
#include "network/bytestring.h"                 // ByteString
#include "network/create.h"                     // create()
#include "network/handle-type.h"                // handle_type
#include "network/socketdata.h"                 // SocketData
#include "network/socketerror.h"                // SocketError
#include "network/sockethints.h"                // SocketHints

#include <memory>       // std::make_shared()

Network::Socket::Socket(SocketHints t_hints, bool t_is_verbose) :
    Socket(create(t_hints, t_is_verbose))
{
}

Network::Socket::Socket(handle_type t_handle, bool t_is_verbose) :
    m_socket_data(std::make_shared<SocketData>(t_handle, t_is_verbose))
{
}

Network::Socket::operator bool() const noexcept
{
    return static_cast<bool>(m_socket_data);
}

Network::Socket::operator handle_type() const
{
    if (!m_socket_data) {
        throw SocketError();
    }

    return static_cast<handle_type>(*m_socket_data);
}

auto Network::Socket::handle() const -> handle_type
{
    if (!m_socket_data) {
        throw SocketError();
    }

    return m_socket_data->handle();
}

auto Network::Socket::is_verbose() const -> bool
{
    if (!m_socket_data) {
        throw SocketError();
    }

    return m_socket_data->is_verbose();
}

auto Network::Socket::peername() const -> ByteString
{
    if (!m_socket_data) {
        throw SocketError();
    }

    return m_socket_data->peername();
}

auto Network::Socket::sockname() const -> ByteString
{
    if (!m_socket_data) {
        throw SocketError();
    }

    return m_socket_data->sockname();
}

auto Network::Socket::is_owner() -> Socket&
{
    if (!m_socket_data) {
        throw SocketError();
    }

    m_socket_data->is_owner();
    return *this;
}
