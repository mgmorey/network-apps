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
#include "network/acceptdata.hpp"       // SocketData
#include "network/bytestring.hpp"       // ByteString

Network::SocketData::SocketData(handle_type t_handle,
                                family_type t_family,
                                const SharedRuntime& t_sr) :
    m_core(t_handle, t_family, t_sr)
{
}

Network::SocketData::SocketData(const AcceptData& t_data) :
    m_core(t_data.core())
{
    m_cache[Symbol::accept] = t_data.accept();
}

auto Network::SocketData::cache(Symbol t_symbol) const noexcept -> ByteString&
{
    return m_cache[t_symbol];
}

auto Network::SocketData::core() const noexcept -> const SocketCore&
{
    return m_core;
}
