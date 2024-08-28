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

#include "network/bytestring.h"         // ByteString
#include "network/handle-type.h"        // handle_type
#include "network/oserrorresult.h"      // OsErrorResult
#include "network/sockethints.h"        // SocketHints

#include <memory>       // std::shared_ptr
#include <ostream>      // std::ostream

namespace Network
{
    class SocketData;

    class Socket
    {
        friend auto bind(Socket& sock, const ByteString& addr) -> OsErrorResult;
        friend auto operator<<(std::ostream& os,
                               const Socket& sock) -> std::ostream&;

    public:
        Socket() noexcept = default;
        explicit Socket(SocketHints t_hints, bool t_is_verbose = false);
        explicit Socket(handle_type t_handle, bool t_is_verbose = false);
        Socket(const Socket&) noexcept = default;
        Socket(Socket&&) noexcept = default;
        ~Socket() = default;
        auto operator=(const Socket&) noexcept -> Socket& = default;
        auto operator=(Socket&&) noexcept -> Socket& = default;
        explicit operator bool() const noexcept;
        explicit operator handle_type() const;
        [[nodiscard]] auto handle() const -> handle_type;
        [[nodiscard]] auto is_verbose() const -> bool;
        [[nodiscard]] auto peername() const -> ByteString;
        [[nodiscard]] auto sockname() const -> ByteString;

    protected:
        auto is_owner() -> Socket&;

    private:
        std::shared_ptr<SocketData> m_socket_data;
    };

    extern auto operator<<(std::ostream& os,
                           const Socket& sock) -> std::ostream&;
}

#endif
