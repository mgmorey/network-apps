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
#include "network/length-type.h"        // length_type
#include "network/optionalhostname.h"   // OptionalHostname
#include "network/to-bytestring-void.h" // to_bytestring()
#include "network/to-sa-length.h"       // to_sa_length()

#ifdef WIN32
#include <ws2tcpip.h>       // addrinfo
#else
#include <netdb.h>          // addrinfo
#endif

Network::SocketHost::SocketHost(const addrinfo& t_ai) :
    m_addr(to_bytestring(t_ai)),
    m_name(to_canonical_name(t_ai))
{
}

auto Network::SocketHost::operator=(const addrinfo& t_ai) -> SocketHost&
{
    m_addr = to_bytestring(t_ai);
    m_name = to_canonical_name(t_ai);
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

auto Network::SocketHost::address() const noexcept -> const Network::ByteString&
{
    return m_addr;
}

auto Network::SocketHost::address_length() const noexcept -> length_type
{
    return m_addr.size();
}

auto Network::SocketHost::canonical_name() const noexcept ->
    const OptionalHostname&
{
    return m_name;
}

auto Network::SocketHost::to_bytestring(const addrinfo& ai) -> ByteString
{
    return Network::to_bytestring(ai.ai_addr, to_sa_length(ai.ai_addrlen));
}

auto Network::SocketHost::to_canonical_name(const addrinfo& t_ai) noexcept ->
    OptionalHostname
{
    if (t_ai.ai_canonname == nullptr) {
        return {};
    }

    return {t_ai.ai_canonname};
}
