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

#include "network/descriptor.h"                 // ByteString,
                                                // Descriptor,
                                                // descriptor_type,
                                                // std::string
#include "network/socket-hint-types.h"          // socket_family_type,
                                                // socket_flags_type,
                                                // socket_protocol_type,
                                                // socket_type_type
#include "network/sockethints.h"                // SocketHints

#include <memory>      // std::shared_ptr
#include <ostream>     // std::ostream

namespace Network
{
    class Socket
    {
    public:
        Socket(socket_family_type t_family,
               socket_type_type t_socktype,
               socket_protocol_type t_protocol = 0,
               socket_flags_type t_flags = 0,
               bool t_pending = false,
               bool t_verbose = false);
        explicit Socket(const SocketHints& t_hints,
                        bool t_pending = false,
                        bool t_verbose = false);
        explicit Socket(descriptor_type t_fd,
                        bool t_pending = false,
                        bool t_verbose = false);
        Socket(const Socket&) noexcept = default;
        Socket(Socket&&) noexcept = default;
        ~Socket() = default;
        auto operator=(const Socket&) noexcept -> Socket& = default;
        auto operator=(Socket&&) noexcept -> Socket& = default;
        explicit operator descriptor_type() const noexcept;
        explicit operator std::string() const;
        auto close() -> Socket&;
        [[nodiscard]] auto is_open() const noexcept -> bool;
        [[nodiscard]] auto peername() const -> ByteString;
        [[nodiscard]] auto sockname() const -> ByteString;

    private:
        std::shared_ptr<Descriptor> m_descriptor;
    };

    extern auto operator<<(std::ostream& os,
                           const Socket& sock) noexcept -> std::ostream&;
}

#endif
