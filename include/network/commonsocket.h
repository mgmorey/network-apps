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
#include "network/oserrorresult.h"      // OsErrorResult
#include "network/socket.h"             // Socket
#include "network/socketapi.h"          // SocketApi
#include "network/socketdata.h"         // SocketData

#include <sys/types.h>      // ssize_t

#include <cstddef>      // std::byte
#include <map>          // std::map
#include <span>         // std::span
#include <string_view>  // std::string_view

namespace Network
{
    class CommonSocket : public Socket
    {
    public:
        explicit CommonSocket(const SocketData& t_sd);

        CommonSocket(const CommonSocket&) noexcept = delete;
        CommonSocket(const CommonSocket&&) noexcept = delete;
        ~CommonSocket() noexcept override;
        auto operator=(const CommonSocket&) noexcept -> CommonSocket& = delete;
        auto operator=(CommonSocket&&) noexcept -> CommonSocket& = delete;

        explicit operator bool() const noexcept;
        explicit operator std::string() const final;

        auto get_name(bool t_is_sockname) const -> std::span<const std::byte>;
        [[nodiscard]] auto is_verbose() const noexcept -> bool;
        [[nodiscard]] auto open(std::span<const std::byte> t_bs,
                                bool is_bind) const -> OsErrorResult;

        [[nodiscard]] auto accept() const -> AcceptResult final;
        [[nodiscard]] auto bind(std::span<const std::byte> t_bs) ->
            OsErrorResult override;
        [[nodiscard]] auto cache(SocketApi api) const ->
            std::span<const std::byte> final;
        [[nodiscard]] auto connect(std::span<const std::byte> t_bs) ->
            OsErrorResult override;
        [[nodiscard]] auto listen(int t_backlog) const -> OsErrorResult final;
        [[nodiscard]] auto peername() const -> std::span<const std::byte> final;
        [[nodiscard]] auto read(std::span<char> t_cs) const -> ssize_t final;
        [[nodiscard]] auto shutdown(int t_how) const -> OsErrorResult final;
        [[nodiscard]] auto sockname() const -> std::span<const std::byte> final;
        [[nodiscard]] auto write(std::string_view t_sv) const -> ssize_t final;

    private:
        mutable std::map<SocketApi, ByteString> m_cache;
        SocketData m_sd;
    };
}

#endif
