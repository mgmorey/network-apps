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

#ifndef NETWORK_FDPAIR_H
#define NETWORK_FDPAIR_H

#include "network/fd.h"                 // Fd, FdPair, SocketHints,
                                        // socket_family_type,
                                        // socket_flags_type,
                                        // socket_protocol_type,
                                        // socket_type_type

#include <array>        // std::array

namespace Network
{
    class FdPair
    {
    public:
        static constexpr auto m_size {2};

#ifndef WIN32
        FdPair(socket_family_type t_family,
               socket_type_type t_socktype,
               socket_protocol_type t_protocol = 0,
               socket_flags_type t_flags = 0,
               bool t_verbose = false);
        explicit FdPair(const SocketHints& t_hints,
                        bool t_verbose = false);
#endif
        FdPair(Fd t_fd1, Fd t_fd2) noexcept;
        FdPair(const FdPair&) noexcept = default;
        FdPair(FdPair&&) noexcept = default;
        FdPair() noexcept = default;
        ~FdPair() noexcept = default;
        auto operator=(const FdPair&) noexcept -> FdPair& = default;
        auto operator=(FdPair&&) noexcept -> FdPair& = default;
        auto operator[](std::size_t i) -> Fd&;
        auto operator[](std::size_t i) const -> const Fd&;

    private:
        std::array<Fd, m_size> m_fd;
    };
}

#endif
