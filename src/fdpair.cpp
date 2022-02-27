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

#include "network/fdpair.h"             // Fd, FdPair
#include "network/get-socketpair.h"     // get_socketpair()

#include <cstddef>      // std::size_t

#ifndef WIN32

Network::FdPair::FdPair(socket_family_type t_family,
                        socket_type_type t_socktype,
                        socket_protocol_type t_protocol,
                        socket_flags_type t_flags,
                        bool t_verbose) :
    FdPair(SocketHints {t_flags, t_family, t_socktype, t_protocol}, t_verbose)
{
}

Network::FdPair::FdPair(const SocketHints& t_hints, bool t_verbose) :
    FdPair(get_socketpair(t_hints, t_verbose))
{
}

#endif

Network::FdPair::FdPair(const Fd& t_fd1, const Fd& t_fd2) noexcept :
    m_fd({t_fd1, t_fd2})
{
}

auto Network::FdPair::at(std::size_t i) const -> const Network::Fd&
{
    return m_fd.at(i);
}

auto Network::FdPair::at(std::size_t i) -> Network::Fd&
{
    return m_fd.at(i);
}
