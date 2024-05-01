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
#include "network/descriptor-null.h"            // descriptor_null
#include "network/descriptor-type.h"            // descriptor_type
#include "network/descriptor.h"                 // Descriptor
#include "network/sockethints.h"                // SocketHints

Network::Socket::Socket(descriptor_type t_handle, bool t_verbose) :
    m_descriptor(new Descriptor {t_handle, t_verbose})
{
}

Network::Socket::Socket(SocketHints t_hints, bool t_verbose) :
    Socket(create(t_hints, t_verbose))
{
}

Network::Socket::operator descriptor_type() const noexcept
{
    return m_descriptor->handle();
}

auto Network::Socket::bound(bool t_bound) -> Socket&
{
    m_descriptor->bound(t_bound);
    return *this;
}

auto Network::Socket::close() -> Socket&
{
    m_descriptor->close();
    return *this;
}

auto Network::Socket::handle() const noexcept -> descriptor_type
{
    return m_descriptor->handle();
}

auto Network::Socket::is_open() const noexcept -> bool
{
    return m_descriptor->handle() != descriptor_null;
}

auto Network::Socket::peername() const -> ByteString
{
    return m_descriptor->peername();
}

auto Network::Socket::sockname() const -> ByteString
{
    return m_descriptor->sockname();
}
