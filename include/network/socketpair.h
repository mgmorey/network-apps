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

#include "network/socket.h"             // Socket
#include "network/sockethints.h"        // SocketHints

#include <array>        // std::array

namespace Network
{
#ifndef WIN32
    class SocketPair
    {
    public:
        static constexpr auto m_size {2};

        explicit SocketPair(const SocketHints& t_hints, bool t_verbose = false);
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
#endif
}

#endif
