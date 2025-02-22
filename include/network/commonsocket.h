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

#include "network/acceptresult.h"               // AcceptResult
#include "network/bytestring.h"                 // ByteString
#include "network/family-type.h"                // family_type
#include "network/handle-type.h"                // handle_type
#include "network/readresult.h"                 // ReadResult
#include "network/socket.h"                     // Socket
#include "network/socketdata.h"                 // SocketData

#ifdef WIN32
#include <winsock2.h>           // AF_UNSPEC
#else
#include <sys/socket.h>         // AF_UNSPEC
#endif

#include <sys/types.h>          // ssize_t

#include <array>        // std::array
#include <string_view>  // std::string_view

namespace Network
{
    class CommonSocket
        : public Socket,
          public SocketData
    {
    public:
        explicit CommonSocket(const SocketData& t_data);
        CommonSocket() noexcept = default;
        CommonSocket(const CommonSocket&) noexcept = delete;
        CommonSocket(const CommonSocket&&) noexcept = delete;
        ~CommonSocket() noexcept override;
        auto operator=(const CommonSocket&) noexcept -> CommonSocket& = delete;
        auto operator=(CommonSocket&&) noexcept -> CommonSocket& = delete;
        explicit operator bool() const noexcept final;
        explicit operator handle_type() const noexcept final;

        [[nodiscard]] auto family() const noexcept -> family_type final;
        [[nodiscard]] auto handle() const noexcept -> handle_type final;
        [[nodiscard]] auto is_verbose() const noexcept -> bool final;

        [[nodiscard]] auto accept() const -> AcceptResult final;
        [[nodiscard]] auto listen(int t_backlog) const ->
            OsErrorResult final;
        [[nodiscard]] auto name(bool t_is_peer) const -> ByteString final;
        [[nodiscard]] auto open(const ByteString& t_addr,
                                bool t_is_bind) -> OsErrorResult override;
        [[nodiscard]] auto read(char* t_data,
                                std::size_t t_size) const -> ssize_t final;
        [[nodiscard]] auto read(std::size_t t_size) const -> ReadResult final;
        [[nodiscard]] auto write(const char* t_data,
                                 std::size_t t_size) const -> ssize_t final;
        [[nodiscard]] auto write(std::string_view t_sv) const -> ssize_t final;

    private:
        mutable std::array<ByteString, 2> m_names;
    };
}

#endif
