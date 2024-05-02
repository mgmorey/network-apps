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
#include "network/descriptor-type.h"            // descriptor_type
#include "network/sockethints.h"                // SocketHints

#include <memory>      // std::shared_ptr
#include <ostream>     // std::ostream

namespace Network
{
    class Descriptor;

    class Socket
    {
        friend auto operator<<(std::ostream& os,
                               const Socket& sock) noexcept -> std::ostream&;

    public:
        Socket() noexcept = default;
        explicit Socket(descriptor_type t_handle, bool t_verbose = false);
        explicit Socket(SocketHints t_hints, bool t_verbose = false);
        Socket(const Socket&) noexcept = default;
        Socket(Socket&&) noexcept = default;
        ~Socket() = default;
        auto operator=(const Socket&) noexcept -> Socket& = default;
        auto operator=(Socket&&) noexcept -> Socket& = default;
        explicit operator descriptor_type() const noexcept;
        explicit operator bool() const noexcept;
        auto bound(bool t_bound) -> Socket&;
        auto close() -> Socket&;
        [[nodiscard]] auto handle() const noexcept -> descriptor_type;
        [[nodiscard]] auto peername() const -> ByteString;
        [[nodiscard]] auto sockname() const -> ByteString;

    private:
        std::shared_ptr<Descriptor> m_descriptor;
    };

    extern auto operator<<(std::ostream& os,
                           const Socket& sock) noexcept -> std::ostream&;
}

#endif
