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

#ifndef NETWORK_UNIXSOCKETDATA_H
#define NETWORK_UNIXSOCKETDATA_H

#include "network/commonsocketdata.h"           // CommonSocketData
#include "network/handle-type.h"                // handle_type
#include "network/socket-family-type.h"         // socket_family_type

namespace Network
{
    class UnixSocketData final
        : public CommonSocketData
    {
    public:
        UnixSocketData() noexcept = default;
        UnixSocketData(socket_family_type t_family,
                       handle_type t_handle,
                       bool t_is_verbose);
        UnixSocketData(const UnixSocketData&) noexcept = delete;
        UnixSocketData(const UnixSocketData&&) noexcept = delete;
        ~UnixSocketData() noexcept final;
        auto operator=(const UnixSocketData&) noexcept -> SocketData& = delete;
        auto operator=(UnixSocketData&&) noexcept -> SocketData& = delete;
        auto is_owner(bool t_is_owner = true) -> SocketData& final;

    private:
        bool m_is_owner {false};
    };
}

#endif
