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

#include "network/acceptdata.hpp"       // AcceptData
#include "network/bytespan.hpp"         // ByteSpan
#include "network/charspan.hpp"         // CharSpan
#include "network/handle-type.hpp"      // handle_type
#include "network/opensymbol.hpp"       // OpenSymbol
#include "network/oserror.hpp"          // OsError
#include "network/symbol.hpp"           // Symbol

#include <sys/types.h>      // ssize_t

#include <ostream>      // std::ostream
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

        explicit virtual operator handle_type() const noexcept = 0;

        [[nodiscard]] virtual auto accept() const -> AcceptData = 0;
        [[nodiscard]] virtual auto get_name(Symbol t_symbol) const ->
            ByteSpan = 0;
        [[nodiscard]] virtual auto listen(int t_backlog) const -> OsError = 0;
        [[nodiscard]] virtual auto open(ByteSpan t_bs, OpenSymbol t_symbol) ->
            OsError = 0;
        [[nodiscard]] virtual auto read(CharSpan t_cs) const -> ssize_t = 0;
        [[nodiscard]] virtual auto shutdown(int t_how) const -> OsError = 0;
        [[nodiscard]] virtual auto write(std::string_view t_sv) const ->
            ssize_t = 0;

        [[nodiscard]] auto bind(ByteSpan t_bs) -> OsError
        {
            return open(t_bs, OpenSymbol::bind);
        }

        [[nodiscard]] auto connect(ByteSpan t_bs) -> OsError
        {
            return open(t_bs, OpenSymbol::connect);
        }

        [[nodiscard]] auto get_peername() const -> ByteSpan
        {
            return get_name(Symbol::getpeername);
        }

        [[nodiscard]] auto get_sockname() const -> ByteSpan
        {
            return get_name(Symbol::getsockname);
        }
    };

    extern auto operator<<(std::ostream& os, const Socket& s) -> std::ostream&;
}

#endif
