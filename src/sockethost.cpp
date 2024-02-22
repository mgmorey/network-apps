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

#include "network/sockethost.h"         // SocketHost
#include "network/bytestring.h"         // ByteString
#include "network/optionalhostname.h"   // OptionalHostname
#include "network/to-bytestring-ai.h"   // to_bytestring()

#ifdef WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

Network::SocketHost::SocketHost(const addrinfo& t_addrinfo) :
    m_addr(to_bytestring(t_addrinfo)),
    m_name(to_canonical_name(t_addrinfo.ai_canonname))
{
}

auto Network::SocketHost::operator=(const addrinfo& t_addrinfo) -> SocketHost&
{
    m_addr = to_bytestring(t_addrinfo);
    m_name = to_canonical_name(t_addrinfo.ai_canonname);
    return *this;
}

auto Network::SocketHost::operator<(const SocketHost& t_host) const noexcept ->
    bool
{
    return m_addr < t_host.m_addr;
}

auto Network::SocketHost::operator>(const SocketHost& t_host) const noexcept ->
    bool
{
    return m_addr > t_host.m_addr;
}

auto Network::SocketHost::operator==(const SocketHost& t_host) const noexcept ->
    bool
{
    return m_addr == t_host.m_addr;
}

auto Network::SocketHost::address() const -> const Network::ByteString&
{
    return m_addr;
}

auto Network::SocketHost::canonical_name() const -> const OptionalHostname&
{
    return m_name;
}

auto Network::SocketHost::to_canonical_name(const char* t_str) noexcept ->
    OptionalHostname
{
    if (t_str == nullptr) {
        return {};
    }

    return {t_str};
}
