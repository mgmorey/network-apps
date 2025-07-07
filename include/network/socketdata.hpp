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

#ifndef NETWORK_SOCKETDATA_HPP
#define NETWORK_SOCKETDATA_HPP

#include "network/bytestring.hpp"       // ByteString
#include "network/family-null.hpp"      // family_null
#include "network/family-type.hpp"      // family_type
#include "network/handle-null.hpp"      // handle_null
#include "network/handle-type.hpp"      // handle_type
#include "network/sharedruntime.hpp"    // SharedRuntime
#include "network/symbol.hpp"           // Symbol

#include <map>          // std::map

namespace Network
{
    class SocketData
    {
    public:
        using Cache = std::map<Symbol, ByteString>;

        SocketData(handle_type t_handle,
                   family_type t_family,
                   const SharedRuntime& t_sr);
        SocketData(const SocketData& t_sd,
                   const ByteString& t_accept,
                   handle_type t_handle);

        SocketData() = delete;
        SocketData(const SocketData&) noexcept = default;
        SocketData(SocketData&&) noexcept = default;
        ~SocketData() noexcept = default;
        auto operator=(const SocketData&) noexcept -> SocketData& = default;
        auto operator=(SocketData&&) noexcept -> SocketData& = default;

        [[nodiscard]] auto cache(Symbol t_symbol) const noexcept -> ByteString&;
        [[nodiscard]] auto family() const noexcept -> family_type;
        [[nodiscard]] auto handle() const noexcept -> handle_type;
        [[nodiscard]] auto runtime() const noexcept -> SharedRuntime;

    private:
        mutable Cache m_bs;
        SharedRuntime m_sr;
        handle_type m_handle {handle_null};
        family_type m_family {family_null};
    };
}

#endif
