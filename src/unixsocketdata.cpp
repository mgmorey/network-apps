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

#include "network/unixsocketdata.h"             // UnixSocketData
#include "network/handle-type.h"                // handle_type
#include "network/remove.h"                     // remove()
#include "network/socket-family-type.h"         // socket_family_type
#include "network/to-path.h"                    // to_path()

Network::UnixSocketData::UnixSocketData(socket_family_type t_family,
                                        handle_type t_handle,
                                        bool t_is_verbose)
    : CommonSocketData(t_family, t_handle, t_is_verbose)
{
}

Network::UnixSocketData::~UnixSocketData() noexcept
{
    state(SocketState::closing);
}

auto Network::UnixSocketData::state(SocketState t_state) -> SocketData&
{
    if (m_state == t_state) {
        return *this;
    }

    if (m_state == SocketState::bound && t_state == SocketState::closing) {
        if (const auto path {to_path(sockname())}) {
            remove(*path, m_is_verbose);
        }
    }

    m_state = t_state;
    return *this;
}
