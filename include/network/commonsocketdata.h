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

#ifndef NETWORK_COMMONSOCKETDATA_H
#define NETWORK_COMMONSOCKETDATA_H

#include "network/bytestring.h"                 // ByteString
#include "network/handle-null.h"                // handle_null
#include "network/handle-type.h"                // handle_type
#include "network/socket-family-type.h"         // socket_family_type
#include "network/socketdata.h"                 // SocketData

#include <optional>     // std::optional

#ifdef WIN32
#include <winsock2.h>       // AF_UNSPEC
#else
#include <sys/socket.h>     // AF_UNSPEC
#endif

namespace Network
{
    class CommonSocketData
        : public SocketData
    {
        friend class UnixSocketData;

    public:
        CommonSocketData() noexcept = default;
        CommonSocketData(socket_family_type t_family,
                         handle_type t_handle,
                         bool t_is_verbose);
        CommonSocketData(const CommonSocketData&) noexcept = delete;
        CommonSocketData(const CommonSocketData&&) noexcept = delete;
        ~CommonSocketData() noexcept override;
        auto operator=(const CommonSocketData&) noexcept -> SocketData& = delete;
        auto operator=(CommonSocketData&&) noexcept -> SocketData& = delete;
        explicit operator bool() const noexcept final;
        explicit operator handle_type() const noexcept final;
        [[nodiscard]] auto family() const noexcept -> socket_family_type final;
        [[nodiscard]] auto handle() const noexcept -> handle_type final;
        auto is_owner(bool t_is_owner = true) -> SocketData& override;
        [[nodiscard]] auto is_verbose() const noexcept -> bool final;
        [[nodiscard]] auto peername() const -> ByteString final;
        [[nodiscard]] auto sockname() const -> ByteString final;

    private:
        mutable std::optional<ByteString> m_peername;
        mutable std::optional<ByteString> m_sockname;
        socket_family_type m_family {AF_UNSPEC};
        handle_type m_handle {handle_null};
        bool m_is_verbose {false};
    };
}

#endif
