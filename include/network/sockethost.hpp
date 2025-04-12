// Copyright (C) 2022  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_SOCKETHOST_H
#define NETWORK_SOCKETHOST_H

#include "network/bytestring.hpp"       // ByteString
#include "network/optionalhostname.hpp" // OptionalHostname

#ifdef WIN32
#include <ws2tcpip.h>       // addrinfo
#else
#include <netdb.h>          // addrinfo
#endif

#include <cstddef>      // std::byte
#include <span>         // std::span

namespace Network
{
    struct SocketHost
    {
        // cppcheck-suppress noExplicitConstructor; NOLINTNEXTLINE
        SocketHost(const addrinfo& t_ai);

        SocketHost() = default;
        SocketHost(const SocketHost&) = default;
        SocketHost(SocketHost&&) = default;
        ~SocketHost() noexcept = default;
        auto operator=(const SocketHost&) -> SocketHost& = default;
        auto operator=(SocketHost&&) -> SocketHost& = default;

        auto operator=(const addrinfo& t_ai) -> SocketHost&;
        auto operator<(const SocketHost& t_host) const noexcept -> bool;
        [[nodiscard]] auto address() const noexcept -> std::span<const std::byte>;
        [[nodiscard]] auto canonical_name() const noexcept ->
            const OptionalHostname&;

    protected:
        static auto to_bytestring(const addrinfo& ai) -> ByteString;
        static auto to_canonical_name(const addrinfo& t_ai) noexcept ->
            OptionalHostname;

    private:
        ByteString m_addr;
        OptionalHostname m_name;
    };
}

#endif
