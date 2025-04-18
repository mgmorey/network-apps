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

#include "network/socketdata.hpp"       // SocketData
#include "network/family-type.hpp"      // family_type
#include "network/handle-null.hpp"      // handle_null
#include "network/handle-type.hpp"      // handle_type
#include "network/logicerror.hpp"       // LogicError

Network::SocketData::SocketData(handle_type t_handle,
                                family_type t_family,
                                bool t_is_verbose) :
    m_handle(t_handle),
    m_family(t_family),
    m_is_verbose(t_is_verbose)
{
    if (m_handle == handle_null) {
        throw LogicError("Invalid socket descriptor value");
    }
}

Network::SocketData::SocketData(const SocketData& t_sd,
                                handle_type t_handle) :
    SocketData(t_handle,
               t_sd.m_family,
               t_sd.m_is_verbose)
{
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

auto Network::SocketData::family(family_type t_family) -> SocketData&
{
    m_family = t_family;
    return *this;
}

auto Network::SocketData::handle(handle_type t_handle) -> SocketData&
{
    m_handle = t_handle;
    return *this;
}

auto Network::SocketData::is_verbose(bool t_is_verbose) -> SocketData&
{
    m_is_verbose = t_is_verbose;
    return *this;
}
