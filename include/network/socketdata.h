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

#ifndef NETWORK_SOCKETDATA_H
#define NETWORK_SOCKETDATA_H

#include "network/bytestring.h"                 // ByteString
#include "network/handle-type.h"                // handle_type
#include "network/socket-family-type.h"         // socket_family_type

namespace Network
{
    class SocketData
    {
    public:
        SocketData() noexcept = default;
        SocketData(const SocketData&) noexcept = delete;
        SocketData(const SocketData&&) noexcept = delete;
        virtual ~SocketData() noexcept = default;
        auto operator=(const SocketData&) noexcept -> SocketData& = delete;
        auto operator=(SocketData&&) noexcept -> SocketData& = delete;
        explicit virtual operator bool() const noexcept = 0;
        explicit virtual operator handle_type() const noexcept = 0;
        [[nodiscard]] virtual auto family() const noexcept -> socket_family_type = 0;
        [[nodiscard]] virtual auto handle() const noexcept -> handle_type = 0;
        virtual auto is_owner(bool t_is_owner = true) -> SocketData& = 0;
        [[nodiscard]] virtual auto is_verbose() const noexcept -> bool = 0;
        [[nodiscard]] virtual auto peername() const -> ByteString = 0;
        [[nodiscard]] virtual auto sockname() const -> ByteString = 0;
    };
}

#endif
