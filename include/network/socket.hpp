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

#ifndef NETWORK_SOCKET_HPP
#define NETWORK_SOCKET_HPP

#include "network/bytespan.hpp"                 // ByteSpan
#include "network/long-handle-type.hpp"         // long_handle_type
#include "network/oserror.hpp"                  // OsError
#include "network/socketdata.hpp"               // SocketData

#include <ostream>      // std::ostream
#include <span>         // std::span
#include <string_view>  // std::string_view

namespace Network
{
    class Socket
    {
    public:
        Socket() noexcept = default;
        Socket(const Socket&) noexcept = delete;
        Socket(Socket&&) noexcept = delete;
        virtual ~Socket() noexcept = default;
        auto operator=(const Socket&) noexcept -> Socket& = delete;
        auto operator=(Socket&&) noexcept -> Socket& = delete;

        explicit virtual operator bool() const noexcept = 0;
        explicit virtual operator long_handle_type() const = 0;

        [[nodiscard]] virtual auto accept() const -> SocketData = 0;
        [[nodiscard]] virtual auto listen(int t_backlog) const -> OsError = 0;
        [[nodiscard]] virtual auto name(bool t_is_sock) const -> ByteSpan = 0;
        [[nodiscard]] virtual auto open(ByteSpan t_bs, bool t_is_bind) ->
            OsError = 0;
        [[nodiscard]] virtual auto read(std::span<char> t_cs) const ->
            ssize_t = 0;
        [[nodiscard]] virtual auto shutdown(int t_how) const -> OsError = 0;
        [[nodiscard]] virtual auto write(std::string_view t_sv) const ->
            ssize_t = 0;

        [[nodiscard]] auto bind(ByteSpan t_bs) -> OsError
        {
            return open(t_bs, true);
        }

        [[nodiscard]] auto connect(ByteSpan t_bs) -> OsError
        {
            return open(t_bs, false);
        }

        [[nodiscard]] auto peername() const -> ByteSpan
        {
            return name(false);
        }

        [[nodiscard]] auto sockname() const -> ByteSpan
        {
            return name(true);
        }
    };

    extern auto operator<<(std::ostream& os,
                           const Socket& s) -> std::ostream&;
}

#endif
