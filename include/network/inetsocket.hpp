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

#include "network/acceptresult.hpp"             // AcceptResult
#include "network/bytespan.hpp"                 // ByteSpan
#include "network/bytestring.hpp"               // ByteString
#include "network/long-handle-type.hpp"         // long_handle_type
#include "network/oserrorresult.hpp"            // OsErrorResult
#include "network/sharedruntime.hpp"            // SharedRuntime
#include "network/socket.hpp"                   // Socket
#include "network/socketdata.hpp"               // SocketData
#include "network/symbols.hpp"                  // Symbols

#include <sys/types.h>      // ssize_t

#include <array>        // std::array
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

        [[nodiscard]] auto accept() const -> AcceptResult final;
        [[nodiscard]] auto bind(ByteSpan t_bs) -> OsErrorResult override;
        [[nodiscard]] auto connect(ByteSpan t_bs) -> OsErrorResult override;
        [[nodiscard]] auto listen(int t_backlog) const -> OsErrorResult final;
        [[nodiscard]] auto peername() const -> ByteSpan final;
        [[nodiscard]] auto read(std::span<char> t_cs) const -> ssize_t final;
        [[nodiscard]] auto shutdown(int t_how) const -> OsErrorResult final;
        [[nodiscard]] auto sockname() const -> ByteSpan final;
        [[nodiscard]] auto write(std::string_view t_sv) const -> ssize_t final;

    protected:
        [[nodiscard]] auto name(bool t_is_sockname) const -> ByteSpan;
        [[nodiscard]] auto open(ByteSpan t_bs,
                                bool is_bind) const -> OsErrorResult;
        [[nodiscard]] auto runtime() const noexcept -> SharedRuntime;

    private:
        mutable std::array<ByteString, symbols.size()> m_names;
        SocketData m_sd;
    };
}

#endif
