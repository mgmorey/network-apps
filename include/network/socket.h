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

#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include "network/sockethints.h"        // SocketHints
#include "network/sockethost.h"         // SocketHost

#ifdef WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

namespace Network
{
    struct Socket
    {
        Socket() noexcept = default;
        explicit Socket(int t_family,
                        int t_socktype = 0,
                        int t_protocol = 0,
                        int t_flags = 0) noexcept;
        // cppcheck-suppress noExplicitConstructor
        Socket(const addrinfo& t_addrinfo) noexcept;  // NOLINT
        Socket(const Socket&) noexcept = default;
        Socket(Socket&&) noexcept = default;
        ~Socket() noexcept = default;
        auto operator=(const Socket&) noexcept -> Socket& = default;
        auto operator=(Socket&&) noexcept -> Socket& = default;
        auto operator=(const addrinfo& t_addrinfo) noexcept -> Socket&;
        auto operator<(const Socket& t_sock) const noexcept -> bool;
        auto operator>(const Socket& t_sock) const noexcept -> bool;
        auto operator==(const Socket& t_sock) const noexcept -> bool;
        [[nodiscard]] auto hints() const noexcept -> const SocketHints&;
        [[nodiscard]] auto host() const noexcept -> const SocketHost&;
        [[nodiscard]] auto flags() const noexcept -> SocketFlags;
        [[nodiscard]] auto family() const noexcept -> SocketFamily;
        [[nodiscard]] auto socktype() const noexcept -> SocketType;
        [[nodiscard]] auto protocol() const noexcept -> SocketProtocol;
        [[nodiscard]] auto address() const noexcept -> ByteString;
        [[nodiscard]] auto canonical_name() const noexcept -> OptionalHostname;

    private:
        SocketHints m_hints;
        SocketHost m_host;
    };

    extern auto operator<<(std::ostream& os,
                           const Socket& sock) noexcept -> std::ostream&;
}

#endif
