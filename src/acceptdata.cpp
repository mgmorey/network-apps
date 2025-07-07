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

#include "network/acceptdata.hpp"       // AcceptData
#include "network/bytestring.hpp"       // ByteString
#include "network/handle-type.hpp"      // handle_type

Network::AcceptData::AcceptData(const ByteString& t_accept,
                                const SocketCore& t_core,
                                handle_type t_handle) :
    m_accept(t_accept), m_core(t_handle, t_core.family(), t_core.runtime())
{
}

auto Network::AcceptData::accept() const noexcept -> const ByteString&
{
    return m_accept;
}

auto Network::AcceptData::core() const noexcept -> const SocketCore&
{
    return m_core;
}
