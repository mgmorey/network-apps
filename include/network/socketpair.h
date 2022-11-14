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

#ifndef NETWORK_SOCKETPAIR_H
#define NETWORK_SOCKETPAIR_H

#include "network/socket.h"             // Socket, SocketHints,
                                        // SocketPair,
                                        // socket_family_type,
                                        // socket_flags_type,
                                        // socket_protocol_type,
                                        // socket_type_type

#include <array>        // std::array

namespace Network
{
    class SocketPair
    {
    public:
        static constexpr auto m_size {2};

#ifndef WIN32
        SocketPair(socket_family_type t_family,
                   socket_type_type t_socktype,
                   socket_protocol_type t_protocol = 0,
                   socket_flags_type t_flags = 0,
                   bool t_verbose = false);
        explicit SocketPair(const SocketHints& t_hints,
                            bool t_verbose = false);
#endif
        SocketPair(const Socket& t_socket1, const Socket& t_socket2) noexcept;
        SocketPair(const SocketPair&) noexcept = default;
        SocketPair(SocketPair&&) noexcept = default;
        ~SocketPair() noexcept = default;
        auto operator=(const SocketPair&) noexcept -> SocketPair& = default;
        auto operator=(SocketPair&&) noexcept -> SocketPair& = default;
        [[nodiscard]] auto at(std::size_t i) const -> const Socket&;
        [[nodiscard]] auto at(std::size_t i) -> Socket&;

    private:
        std::array<Socket, m_size> m_socket;
    };
}

#endif
