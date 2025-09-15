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
#include "network/socketcore.hpp"       // SocketCore

Network::AcceptData::AcceptData(const ByteString& t_nm,
                                const SocketCore& t_sc,
                                handle_type t_handle) :
    m_sc(t_sc, t_handle), m_nm(t_nm)
{
}

auto Network::AcceptData::core() const noexcept -> const SocketCore&
{
    return m_sc;
}

auto Network::AcceptData::name() const noexcept -> const ByteString&
{
    return m_nm;
}
