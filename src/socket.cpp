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

#include "network/socket.h"             // Descriptor, Socket,
                                        // descriptor_null,
                                        // descriptor_type
#include "network/create-socket.h"      // create_socket()
#include "network/get-peername.h"       // get_peername()
#include "network/get-sockname.h"       // get_sockname()

#include <string>       // std::string

Network::Socket::Socket(socket_family_type t_family,
                        socket_type_type t_socktype,
                        socket_protocol_type t_protocol,
                        socket_flags_type t_flags,
                        bool t_pending,
                        bool t_verbose) :
    Socket(SocketHints {t_flags, t_family, t_socktype, t_protocol},
           t_pending,
           t_verbose)
{
}

Network::Socket::Socket(const SocketHints& t_hints,
                        bool t_pending,
                        bool t_verbose) :
    Socket(create_socket(t_hints, t_pending, t_verbose))
{
}

Network::Socket::Socket(descriptor_type t_handle,
                        bool t_pending,
                        bool t_verbose) :
    m_descriptor(new Descriptor {t_handle, t_pending, t_verbose})
{
}

Network::Socket::operator descriptor_type() const noexcept
{
    return m_descriptor->handle();
}

auto Network::Socket::close() -> Socket&
{
    m_descriptor->close();
    return *this;
}

auto Network::Socket::is_open() const noexcept -> bool
{
    return m_descriptor->handle() != descriptor_null;
}

auto Network::Socket::peername() const -> ByteString
{
    return get_peername(m_descriptor->handle(), m_descriptor->verbose());
}

auto Network::Socket::sockname() const -> ByteString
{
    return get_sockname(m_descriptor->handle(), m_descriptor->verbose());
}
