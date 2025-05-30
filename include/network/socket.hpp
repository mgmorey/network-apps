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

#include "network/acceptresult.hpp"     // AcceptResult
#include "network/oserrorresult.hpp"    // OsErrorResult
#include "network/symbol.hpp"           // Symbol

#include <cstddef>      // std::byte
#include <ostream>      // std::ostream
#include <span>         // std::span

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

        explicit virtual operator std::string() const = 0;
        [[nodiscard]] virtual auto accept() const -> AcceptResult = 0;
        [[nodiscard]] virtual auto bind(std::span<const std::byte> t_bs) ->
            OsErrorResult = 0;
        [[nodiscard]] virtual auto cache(Symbol symbol) const ->
            std::span<const std::byte> = 0;
        [[nodiscard]] virtual auto connect(std::span<const std::byte> t_bs) ->
            OsErrorResult = 0;
        [[nodiscard]] virtual auto listen(int t_backlog) const ->
            OsErrorResult = 0;
        [[nodiscard]] virtual auto peername() const ->
            std::span<const std::byte> = 0;
        [[nodiscard]] virtual auto read(std::span<char> t_cs) const ->
            ssize_t = 0;
        [[nodiscard]] virtual auto shutdown(int t_how) const ->
            OsErrorResult = 0;
        [[nodiscard]] virtual auto sockname() const ->
            std::span<const std::byte> = 0;
        [[nodiscard]] virtual auto write(std::string_view t_sv) const ->
            ssize_t = 0;
    };

    extern auto operator<<(std::ostream& os,
                           const Socket& sock) -> std::ostream&;
}

#endif
