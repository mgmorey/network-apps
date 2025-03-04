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

#include "network/acceptresult.h"       // AcceptResult
#include "network/bytestring.h"         // ByteString
#include "network/family-null.h"        // family_null
#include "network/family-type.h"        // family_type
#include "network/handle-null.h"        // handle_null
#include "network/handle-type.h"        // handle_type
#include "network/readresult.h"         // ReadResult
#include "network/socket.h"             // Socket
#include "network/socketdata.h"         // SocketData

#include <sys/types.h>          // ssize_t

#include <array>        // std::array
#include <cstddef>      // std::byte
#include <span>         // std::span
#include <string_view>  // std::string_view

namespace Network
{
    class CommonSocket : public Socket
    {
    public:
        CommonSocket(handle_type t_handle,
                     family_type t_family,
                     bool t_is_verbose = false,
                     bool t_is_testing = false);
        CommonSocket(const SocketData& t_sd, handle_type t_handle);
        explicit CommonSocket(const SocketData& t_sd);

        CommonSocket(const CommonSocket&) noexcept = delete;
        CommonSocket(const CommonSocket&&) noexcept = delete;
        ~CommonSocket() noexcept override;
        auto operator=(const CommonSocket&) noexcept -> CommonSocket& = delete;
        auto operator=(CommonSocket&&) noexcept -> CommonSocket& = delete;

        [[nodiscard]] auto family() const noexcept -> family_type override;
        [[nodiscard]] auto handle() const noexcept -> handle_type override;
        [[nodiscard]] auto is_verbose() const noexcept -> bool override;
        [[nodiscard]] auto is_testing() const noexcept -> bool override;

        [[nodiscard]] auto accept() const -> AcceptResult final;
        [[nodiscard]] auto listen(int t_backlog) const ->
            OsErrorResult final;
        [[nodiscard]] auto name(bool t_is_peer) const -> ByteString final;
        [[nodiscard]] auto open(std::span<const std::byte> t_bs,
                                bool t_is_bind) -> OsErrorResult override;
        [[nodiscard]] auto read(char* t_data,
                                std::size_t t_size) const -> ssize_t final;
        [[nodiscard]] auto read(std::size_t t_size) const -> ReadResult final;
        [[nodiscard]] auto shutdown(int t_how) const -> OsErrorResult final;
        [[nodiscard]] auto write(const char* t_data,
                                 std::size_t t_size) const -> ssize_t final;
        [[nodiscard]] auto write(std::string_view t_sv) const -> ssize_t final;

    private:
        mutable std::array<ByteString, 2> m_names;
        handle_type m_handle {handle_null};
        family_type m_family {family_null};
        bool m_is_verbose {false};
        bool m_is_testing {false};
    };
}

#endif
