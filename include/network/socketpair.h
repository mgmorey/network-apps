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

#include "network/socket.h"                     // Socket
#include "network/sockethints.h"                // SocketHints
#include "network/socketpair-size.h"            // socketpair_size

#include <array>        // std::array
#include <cstddef>      // std::size_t

namespace Network
{
#ifndef WIN32
    class SocketPair
    {
    public:
        using size_type = std::size_t;
        using value_type = std::array<Socket, socketpair_size>;

        explicit SocketPair(const SocketHints& t_hints, bool t_is_verbose = false);
        SocketPair(const Socket& t_socket_1, const Socket& t_socket_2) noexcept;
        SocketPair(const SocketPair&) noexcept = default;
        SocketPair(SocketPair&&) noexcept = default;
        ~SocketPair() noexcept = default;
        auto operator=(const SocketPair&) noexcept -> SocketPair& = default;
        auto operator=(SocketPair&&) noexcept -> SocketPair& = default;
        [[nodiscard]] auto at(size_type t_index) const -> const Socket&;
        [[nodiscard]] auto at(size_type t_index) -> Socket&;

    private:
        value_type m_socket_pair;
    };
#endif
}

#endif
