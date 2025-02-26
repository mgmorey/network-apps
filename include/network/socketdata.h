// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_SOCKETDATA_H
#define NETWORK_SOCKETDATA_H

#include "network/family-type.h"        // family_type
#include "network/handle-null.h"        // handle_null
#include "network/handle-type.h"        // handle_type

#ifdef WIN32
#include <winsock2.h>           // AF_UNSPEC
#else
#include <sys/socket.h>         // AF_UNSPEC
#endif

namespace Network
{
    class SocketData
    {
    public:
        SocketData(handle_type t_handle,
                   family_type t_family,
                   bool t_is_verbose = false,
                   bool t_is_testing = false);
        SocketData(const SocketData& t_socket,
                   handle_type t_handle);

        SocketData() noexcept = default;
        SocketData(const SocketData&) noexcept = default;
        SocketData(SocketData&&) noexcept = default;
        ~SocketData() noexcept = default;
        auto operator=(const SocketData&) noexcept -> SocketData& = default;
        auto operator=(SocketData&&) noexcept -> SocketData& = default;
        explicit operator bool() const noexcept;
        explicit operator handle_type() const noexcept;

        [[nodiscard]] auto family() const noexcept -> family_type;
        [[nodiscard]] auto handle() const noexcept -> handle_type;
        [[nodiscard]] auto is_verbose() const noexcept -> bool;

    private:
        handle_type m_handle {handle_null};
        family_type m_family {AF_UNSPEC};
        bool m_is_verbose {false};
        bool m_is_testing {false};
    };
}

#endif
