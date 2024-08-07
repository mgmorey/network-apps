// Copyright (C) 2022  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include "network/bytestring.h"                 // ByteString
#include "network/socket-type.h"                // socket_type
#include "network/sockethints.h"                // SocketHints

#include <memory>      // std::shared_ptr
#include <ostream>     // std::ostream

namespace Network
{
    class SocketData;

    class Socket
    {
        friend auto operator<<(std::ostream& os,
                               const Socket& sock) noexcept -> std::ostream&;

    public:
        Socket() noexcept = default;
        explicit Socket(socket_type t_socket, bool t_verbose = false);
        explicit Socket(SocketHints t_hints, bool t_verbose = false);
        Socket(const Socket&) noexcept = default;
        Socket(Socket&&) noexcept = default;
        ~Socket() = default;
        auto operator=(const Socket&) noexcept -> Socket& = default;
        auto operator=(Socket&&) noexcept -> Socket& = default;
        explicit operator socket_type() const noexcept;
        explicit operator bool() const noexcept;
        auto bound() -> Socket&;
        [[nodiscard]] auto peername() const -> ByteString;
        [[nodiscard]] auto sockname() const -> ByteString;

    private:
        std::shared_ptr<SocketData> m_socket_data;
    };

    extern auto operator<<(std::ostream& os,
                           const Socket& sock) noexcept -> std::ostream&;
}

#endif
