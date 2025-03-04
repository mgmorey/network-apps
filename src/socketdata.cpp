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

#include "network/socketdata.h"         // SocketData
#include "network/family-type.h"        // family_type
#include "network/familyerror.h"        // FamilyError
#include "network/handle-null.h"        // handle_null
#include "network/handle-type.h"        // handle_type
#include "network/logicerror.h"         // LogicError

#ifdef WIN32
#include <winsock2.h>           // AF_UNSPEC
#else
#include <sys/socket.h>         // AF_UNSPEC
#endif

Network::SocketData::SocketData(handle_type t_handle,
                                family_type t_family,
                                bool t_is_verbose,
                                bool t_is_testing)
    : m_family(t_family),
      m_is_verbose(t_is_verbose),
      m_is_testing(t_is_testing)
{
    static_cast<void>(handle(t_handle));
}

Network::SocketData::SocketData(const SocketData& t_socket,
                                handle_type t_handle)
    : SocketData(t_handle,
                 t_socket.m_family,
                 t_socket.m_is_verbose,
                 t_socket.m_is_testing)
{
}

Network::SocketData::operator bool() const noexcept
{
    return m_handle != handle_null;
}

Network::SocketData::operator handle_type() const noexcept
{
    return m_handle;
}

auto Network::SocketData::family() const noexcept -> family_type
{
    return m_family;
}

auto Network::SocketData::family(family_type t_family) -> SocketData&
{
    if (!m_is_testing && t_family == AF_UNSPEC) {
        throw FamilyError(t_family);
    }

    m_family = t_family;
    return *this;
}

auto Network::SocketData::handle() const noexcept -> handle_type
{
    return m_handle;
}

auto Network::SocketData::handle(handle_type t_handle) -> SocketData&
{
    if (!m_is_testing && t_handle == handle_null) {
        throw LogicError("Invalid socket descriptor value");
    }

    m_handle = t_handle;
    return *this;
}

auto Network::SocketData::is_verbose() const noexcept -> bool
{
    return m_is_verbose;
}
