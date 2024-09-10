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

#ifndef NETWORK_COMMONSOCKET_H
#define NETWORK_COMMONSOCKET_H

#include "network/bytestring.h"                 // ByteString
#include "network/handle-null.h"                // handle_null
#include "network/handle-type.h"                // handle_type
#include "network/socket-family-type.h"         // socket_family_type
#include "network/socket.h"                     // Socket

#ifdef WIN32
#include <winsock2.h>       // AF_UNSPEC
#else
#include <sys/socket.h>     // AF_UNSPEC
#endif

#include <array>        // std::array

namespace Network
{
    class CommonSocket
        : public Socket
    {
    public:
        CommonSocket() noexcept = default;
        CommonSocket(socket_family_type t_family,
                     handle_type t_handle,
                     bool t_is_verbose);
        CommonSocket(const CommonSocket&) noexcept = delete;
        CommonSocket(const CommonSocket&&) noexcept = delete;
        ~CommonSocket() noexcept override;
        auto operator=(const CommonSocket&) noexcept -> CommonSocket& = delete;
        auto operator=(CommonSocket&&) noexcept -> CommonSocket& = delete;
        explicit operator bool() const noexcept final;
        explicit operator handle_type() const noexcept final;
        [[nodiscard]] auto family() const noexcept -> socket_family_type final;
        [[nodiscard]] auto handle() const noexcept -> handle_type final;
        [[nodiscard]] auto is_verbose() const noexcept -> bool final;
        [[nodiscard]] auto
        listen(int backlog_size) const -> OsErrorResult final;
        [[nodiscard]] auto name(bool t_is_peer) const -> ByteString final;
        [[nodiscard]] auto open(const ByteString& t_addr,
                                bool t_is_bind) -> OsErrorResult override;
        [[nodiscard]] auto read(char* data,
                                std::size_t size) -> ssize_t final;
        [[nodiscard]] auto write(const char* data,
                                 std::size_t size) -> ssize_t final;

    private:
        mutable std::array<ByteString, 2> m_names;
        socket_family_type m_family {AF_UNSPEC};
        handle_type m_handle {handle_null};
        bool m_is_verbose {false};
    };
}

#endif
