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

#ifndef NETWORK_INETSOCKET_HPP
#define NETWORK_INETSOCKET_HPP

#include "network/acceptdata.hpp"               // AcceptData
#include "network/bytespan.hpp"                 // ByteSpan
#include "network/long-handle-type.hpp"         // long_handle_type
#include "network/oserror.hpp"                  // OsError
#include "network/sharedruntime.hpp"            // SharedRuntime
#include "network/socket.hpp"                   // Socket
#include "network/socketdata.hpp"               // SocketData

#include <sys/types.h>      // ssize_t

#include <span>         // std::span
#include <string_view>  // std::string_view

namespace Network
{
    class InetSocket : public Socket
    {
    public:
        explicit InetSocket(const SocketData& t_sd);

        InetSocket(const InetSocket&) noexcept = delete;
        InetSocket(InetSocket &&) noexcept = delete;
        ~InetSocket() noexcept override;
        auto operator=(const InetSocket&) noexcept -> InetSocket& = delete;
        auto operator=(InetSocket&&) noexcept -> InetSocket& = delete;

        explicit operator bool() const noexcept final;
        explicit operator long_handle_type() const final;

        [[nodiscard]] auto accept() const -> AcceptData final;
        [[nodiscard]] auto listen(int t_backlog) const -> OsError final;
        [[nodiscard]] auto name(bool t_is_sock) const -> ByteSpan final;
        [[nodiscard]] auto open(ByteSpan t_bs, bool t_is_bind) -> OsError override;
        [[nodiscard]] auto read(std::span<char> t_cs) const -> ssize_t final;
        [[nodiscard]] auto shutdown(int t_how) const -> OsError final;
        [[nodiscard]] auto write(std::string_view t_sv) const -> ssize_t final;

    protected:
        [[nodiscard]] auto runtime() const noexcept -> SharedRuntime;

    private:
        const SocketData m_sd;
    };
}

#endif
