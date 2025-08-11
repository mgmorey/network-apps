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

#ifndef NETWORK_ACCEPTDATA_HPP
#define NETWORK_ACCEPTDATA_HPP

#include "network/bytestring.hpp"       // ByteString
#include "network/handle-type.hpp"      // handle_type
#include "network/socketcore.hpp"       // SocketCore

namespace Network
{
    struct AcceptData
    {
        AcceptData(const ByteString& t_bs,
                   const SocketCore& t_sc,
                   handle_type t_handle);

        AcceptData() = delete;
        AcceptData(const AcceptData&) noexcept = default;
        AcceptData(AcceptData&&) noexcept = default;
        ~AcceptData() noexcept = default;
        auto operator=(const AcceptData&) noexcept -> AcceptData& = default;
        auto operator=(AcceptData&&) noexcept -> AcceptData& = default;

        [[nodiscard]] auto accept() const noexcept -> const ByteString&;
        [[nodiscard]] auto core() const noexcept -> const SocketCore&;

    private:
        ByteString m_bs;
        SocketCore m_sc;
    };
}

#endif
