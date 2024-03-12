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

#include "network/bytestring.h"         // ByteString
#include "network/optionalhostname.h"   // OptionalHostname

#ifdef WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

namespace Network
{
    struct SocketHost
    {
        SocketHost() = default;
        // cppcheck-suppress noExplicitConstructor
        SocketHost(const addrinfo& t_ai);  // NOLINT
        SocketHost(const SocketHost&) = default;
        SocketHost(SocketHost&&) = default;
        ~SocketHost() noexcept = default;
        auto operator=(const addrinfo& t_ai) -> SocketHost&;
        auto operator=(const SocketHost&) -> SocketHost& = default;
        auto operator=(SocketHost&&) -> SocketHost& = default;
        auto operator<(const SocketHost& t_host) const noexcept -> bool;
        auto operator>(const SocketHost& t_host) const noexcept -> bool;
        auto operator==(const SocketHost& t_host) const noexcept -> bool;
        [[nodiscard]] auto address() const -> const ByteString&;
        [[nodiscard]] auto canonical_name() const -> const OptionalHostname&;

    protected:
        static auto to_canonical_name(const char* t_str) noexcept ->
            OptionalHostname;

    private:
        ByteString m_addr;
        OptionalHostname m_name;
    };
}

#endif
