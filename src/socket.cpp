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

#include "network/socket.h"             // Hints, Host, Socket

#ifdef WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

Network::Socket::Socket(int t_family,
                        int t_socktype,
                        int t_protocol,
                        int t_flags) noexcept :
    m_hints(t_family, t_socktype, t_protocol, t_flags)
{
}

Network::Socket::Socket(const addrinfo& t_addrinfo) noexcept :
    m_hints(t_addrinfo),
    m_host(t_addrinfo)
{
}

auto Network::Socket::operator=(const addrinfo& t_addrinfo) noexcept ->
    Network::Socket&
{
    m_hints = t_addrinfo;
    m_host = t_addrinfo;
    return *this;
}

auto Network::Socket::operator<(const Socket& t_sock) const noexcept -> bool
{
    return (hints() < t_sock.hints() ||
            host() < t_sock.host());
}

auto Network::Socket::operator>(const Socket& t_sock) const noexcept -> bool
{
    return (hints() > t_sock.hints() ||
            host() > t_sock.host());
}

auto Network::Socket::operator==(const Socket& t_sock) const noexcept -> bool
{
    return (hints() == t_sock.hints() &&
            host() == t_sock.host());
}

auto Network::Socket::hints() const noexcept -> const Hints&
{
    return m_hints;
}

auto Network::Socket::host() const noexcept -> const Host&
{
    return m_host;
}

auto Network::Socket::flags() const noexcept -> Flags
{
    return m_hints.flags();
}

auto Network::Socket::family() const noexcept -> Family
{
    return m_hints.family();
}

auto Network::Socket::socktype() const noexcept -> SockType
{
    return m_hints.socktype();
}

auto Network::Socket::protocol() const noexcept -> Protocol
{
    return m_hints.protocol();
}

auto Network::Socket::address() const noexcept -> ByteString
{
    return m_host.address();
}

auto Network::Socket::canonical_name() const noexcept -> OptionalHostname
{
    return m_host.canonical_name();
}
