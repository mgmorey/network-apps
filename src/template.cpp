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

#include "network/template.h"                   // Template
#include "network/bytestring.h"                 // ByteString
#include "network/optionalhostname.h"           // OptionalHostname
#include "network/socketfamily.h"               // SocketFamily
#include "network/socketflags.h"                // SocketFlags
#include "network/sockethints.h"                // SocketHints
#include "network/sockethost.h"                 // SocketHost
#include "network/socketprotocol.h"             // SocketProtocol
#include "network/sockettype.h"                 // SocketType

#ifdef WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

Network::Template::Template(const addrinfo& t_ai) noexcept :
    m_hints(t_ai),
    m_host(t_ai)
{
}

auto Network::Template::operator=(const addrinfo& t_ai) noexcept -> Template&
{
    m_hints = t_ai;
    m_host = t_ai;
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

auto Network::Template::address() const noexcept -> const ByteString&
{
    return m_host.address();
}

auto Network::Template::canonical_name() const noexcept ->
    const OptionalHostname&
{
    return m_host.canonical_name();
}
