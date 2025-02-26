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
#include "network/handle-null.h"        // handle_null
#include "network/handle-type.h"        // handle_type
#include "network/logicerror.h"         // LogicError

Network::SocketData::SocketData(handle_type t_handle,
                                family_type t_family,
                                bool t_is_verbose,
                                bool t_is_testing)
    : m_handle(t_handle), m_family(t_family),
      m_is_verbose(t_is_verbose),
      m_is_testing(t_is_testing)
{
    if (!m_is_testing && m_handle == handle_null) {
        throw LogicError("Invalid socket descriptor value");
    }
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

auto Network::SocketData::handle() const noexcept -> handle_type
{
    return m_handle;
}

auto Network::SocketData::is_verbose() const noexcept -> bool
{
    return m_is_verbose;
}
