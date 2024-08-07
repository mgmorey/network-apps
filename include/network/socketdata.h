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
#include "network/socket-null.h"                // socket_null
#include "network/socket-type.h"                // socket_type

#include <optional>     // std::optional

namespace Network
{
    class SocketData
    {
    public:
        SocketData() noexcept = default;
        explicit SocketData(socket_type t_socket, bool t_verbose);
        SocketData(const SocketData&) noexcept = delete;
        SocketData(SocketData&&) noexcept = delete;
        ~SocketData() noexcept;
        auto operator=(const SocketData&) noexcept -> SocketData& = delete;
        auto operator=(SocketData&&) noexcept -> SocketData& = delete;
        explicit operator socket_type() const noexcept;
        auto bound() -> SocketData&;
        [[nodiscard]] auto peername() const -> ByteString;
        [[nodiscard]] auto sockname() const -> ByteString;

    protected:
        auto bound(bool t_is_bound) -> SocketData&;
        auto close() const -> const SocketData&;

    private:
        mutable std::optional<ByteString> m_peername;
        mutable std::optional<ByteString> m_sockname;
        socket_type m_socket {socket_null};
        bool m_is_bound {false};
        bool m_verbose {false};
    };
}

#endif
