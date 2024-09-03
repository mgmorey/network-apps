// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/commonsocketdata.h"           // CommonSocketData
#include "network/bytestring.h"                 // ByteString
#include "network/get-peername.h"               // get_peername()
#include "network/get-sockname.h"               // get_sockname()
#include "network/handle-null.h"                // handle_null
#include "network/handle-type.h"                // handle_type
#include "network/logicerror.h"                 // LogicError

#include <string>       // std::to_string()

Network::CommonSocketData::CommonSocketData(handle_type t_handle,
                                            bool t_is_verbose)
    : m_handle(t_handle),
      m_is_verbose(t_is_verbose)
{
    if (m_handle == handle_null) {
        throw LogicError("Invalid socket descriptor value: " +
                         std::to_string(m_handle));
    }
}

Network::CommonSocketData::operator bool() const noexcept
{
    return m_handle != handle_null;
}

Network::CommonSocketData::operator handle_type() const noexcept
{
    return m_handle;
}

auto Network::CommonSocketData::handle() const noexcept -> handle_type
{
    return m_handle;
}

auto Network::CommonSocketData::is_owner(bool t_is_owner) -> SocketData&
{
    static_cast<void>(t_is_owner);
    return *this;
}

auto Network::CommonSocketData::is_verbose() const noexcept -> bool
{
    return m_is_verbose;
}

auto Network::CommonSocketData::peername() const -> ByteString
{
    if (!m_peername) {
        m_peername = get_peername(m_handle, m_is_verbose);
    }

    return *m_peername;
}

auto Network::CommonSocketData::sockname() const -> ByteString
{
    if (!m_sockname) {
        m_sockname = get_sockname(m_handle, m_is_verbose);
    }

    return *m_sockname;
}
