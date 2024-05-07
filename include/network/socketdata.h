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
#include "network/optionalbytestring.h"         // OptionalByteString
#include "network/socket-null.h"                // socket_null
#include "network/socket-type.h"                // socket_type

#ifndef WIN32
#include "network/optionalpathname.h"           // OptionalPathanme
#endif

namespace Network
{
    class SocketData
    {
    public:
        SocketData() noexcept = default;
        explicit SocketData(socket_type t_socket, bool t_verbose) noexcept;
        SocketData(const SocketData&) noexcept = delete;
        SocketData(SocketData&&) noexcept = delete;
        ~SocketData() noexcept;
        auto operator=(const SocketData&) noexcept -> SocketData& = delete;
        auto operator=(SocketData&&) noexcept -> SocketData& = delete;
        explicit operator socket_type() const noexcept;
        explicit operator bool() const noexcept;
        auto bound() -> SocketData&;
        auto bound(bool t_bound) -> SocketData&;
        auto close() -> SocketData&;
        [[nodiscard]] auto peername() const -> ByteString;
        [[nodiscard]] auto sockname() const -> ByteString;

    private:
        mutable OptionalByteString m_peername;
        mutable OptionalByteString m_sockname;
#ifndef WIN32
        OptionalPathname m_sockpath;
#endif
        socket_type m_socket {socket_null};
        bool m_verbose {false};
    };
}

#endif
