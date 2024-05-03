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
#include "network/logicerror.h"                 // LogicError
#include "network/socket-null.h"                // socket_null
#include "network/socket-type.h"                // socket_type
#include "network/socketdata.h"                 // SocketData
#include "network/sockethints.h"                // SocketHints

Network::Socket::Socket(socket_type t_socket, bool t_verbose) :
    m_socket_data(new SocketData {t_socket, t_verbose})
{
}

Network::Socket::Socket(SocketHints t_hints, bool t_verbose) :
    Socket(create(t_hints, t_verbose))
{
}

Network::Socket::operator socket_type() const noexcept
{
    if (!m_socket_data) {
        return socket_null;
    }

    return static_cast<socket_type>(*m_socket_data);
}

Network::Socket::operator bool() const noexcept
{
    if (!m_socket_data) {
        return false;
    }

    return static_cast<bool>(*m_socket_data);
}

auto Network::Socket::bound() -> Socket&
{
    if (!m_socket_data) {
        return *this;
    }

    m_socket_data->bound(true);
    return *this;
}

auto Network::Socket::close() -> Socket&
{
    m_socket_data.reset();
    return *this;
}

auto Network::Socket::peername() const -> ByteString
{
    if (!m_socket_data) {
        throw LogicError("No socket available for peername()");
    }

    return m_socket_data->peername();
}

auto Network::Socket::sockname() const -> ByteString
{
    if (!m_socket_data) {
        throw LogicError("No socket available for sockname()");
    }

    return m_socket_data->sockname();
}
