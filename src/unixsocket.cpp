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

#include "network/unixsocket.h"                 // UnixSocket
#include "network/handle-type.h"                // handle_type
#include "network/socket-family-type.h"         // socket_family_type
#include "network/to-path.h"                    // to_path()

#include <filesystem>   // std::filesystem
#include <iostream>     // std::cout, std::endl

Network::UnixSocket::UnixSocket(socket_family_type t_family,
                                handle_type t_handle,
                                bool t_is_verbose)
    : CommonSocket(t_family, t_handle, t_is_verbose)
{
}

Network::UnixSocket::~UnixSocket() noexcept
{
    state(SocketState::closing);
}

auto Network::UnixSocket::state(SocketState t_state) -> GenericSocket&
{
    if (m_state == t_state) {
        return *this;
    }

    if (m_state == SocketState::bound && t_state == SocketState::closing) {
        if (const auto path {to_path(sockname())}) {
            if (m_is_verbose) {
                std::cout << "Calling std::filesystem::remove("
                          << *path
                          << ')'
                          << std::endl;
            }

            std::filesystem::remove(*path);
        }
    }

    m_state = t_state;
    return *this;
}
