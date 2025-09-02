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
#include "network/acceptdata.hpp"       // AcceptData
#include "network/family-type.hpp"      // family_type
#include "network/handle-type.hpp"      // handle_type
#include "network/sharedruntime.hpp"    // SharedRuntime
#include "network/socketcore.hpp"       // SocketCore
#include "network/symbol.hpp"           // Symbol

Network::SocketData::SocketData(handle_type t_handle,
                                family_type t_family,
                                const SharedRuntime& t_sr) :
    m_sc(t_handle, t_family, t_sr)
{
}

Network::SocketData::SocketData(const AcceptData& t_ad) : m_sc(t_ad.core())
{
    m_nc[Symbol::accept] = t_ad.name();
}

auto Network::SocketData::core() const noexcept -> const SocketCore&
{
    return m_sc;
}

auto Network::SocketData::name(Symbol t_symbol) const noexcept -> Name&
{
    return m_nc[t_symbol];
}
