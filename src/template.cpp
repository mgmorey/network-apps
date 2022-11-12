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

#include "network/template.h"                   // SocketHints,
                                                // SocketHost,
                                                // Template

#ifdef WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

Network::Template::Template(int t_family,
                            int t_socktype,
                            int t_protocol,
                            int t_flags) :
    m_hints(t_family, t_socktype, t_protocol, t_flags)
{
}

Network::Template::Template(const addrinfo& t_addrinfo) noexcept :
    m_hints(t_addrinfo),
    m_host(t_addrinfo)
{
}

auto Network::Template::operator=(const addrinfo& t_addrinfo) noexcept ->
    Network::Template&
{
    m_hints = t_addrinfo;
    m_host = t_addrinfo;
    return *this;
}

auto Network::Template::operator<(const Template& t_sock) const noexcept -> bool
{
    return (hints() < t_sock.hints() ||
            host() < t_sock.host());
}

auto Network::Template::operator>(const Template& t_sock) const noexcept -> bool
{
    return (hints() > t_sock.hints() ||
            host() > t_sock.host());
}

auto Network::Template::operator==(const Template& t_sock) const noexcept -> bool
{
    return (hints() == t_sock.hints() &&
            host() == t_sock.host());
}

auto Network::Template::hints() const noexcept -> const SocketHints&
{
    return m_hints;
}

auto Network::Template::host() const noexcept -> const SocketHost&
{
    return m_host;
}

auto Network::Template::flags() const noexcept -> SocketFlags
{
    return m_hints.flags();
}

auto Network::Template::family() const noexcept -> SocketFamily
{
    return m_hints.family();
}

auto Network::Template::socktype() const noexcept -> SocketType
{
    return m_hints.socktype();
}

auto Network::Template::protocol() const noexcept -> SocketProtocol
{
    return m_hints.protocol();
}

auto Network::Template::address() const noexcept -> const ByteString&
{
    return m_host.address();
}

auto Network::Template::canonical_name() const noexcept ->
    const OptionalHostname&
{
    return m_host.canonical_name();
}
