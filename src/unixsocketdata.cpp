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
#include "network/close.h"                      // close()
#include "network/get-sockname.h"               // get_sockname()
#include "network/handle-type.h"                // handle_type
#include "network/remove.h"                     // remove()
#include "network/socket-family-type.h"         // socket_family_type
#include "network/to-path.h"                    // to_path()

#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::endl

Network::UnixSocketData::UnixSocketData(socket_family_type t_family,
                                        handle_type t_handle,
                                        bool t_is_verbose)
    : CommonSocketData(t_family, t_handle, t_is_verbose)
{
}

Network::UnixSocketData::~UnixSocketData() noexcept
{
    try {
        close();
        is_owner(false);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}

auto Network::UnixSocketData::close() const -> const SocketData&
{
    if (const auto result {Network::close(m_handle, m_is_verbose)}) {
        std::cerr << result.string()
                  << std::endl;
    }

    return *this;
}

auto Network::UnixSocketData::is_owner(bool t_is_owner) -> SocketData&
{
    if (m_is_owner == t_is_owner) {
        return *this;
    }

    if (t_is_owner) {
        if (!m_sockname) {
            m_sockname = get_sockname(m_handle, m_is_verbose);
        }
    } else if (m_sockname) {
        if (const auto path {to_path(*m_sockname)}) {
            remove(*path, m_is_verbose);
        }
    }

    m_is_owner = t_is_owner;
    return *this;
}
