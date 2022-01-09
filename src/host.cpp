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

#include "network/host.h"               // Bytes, Host, Hostname,
                                        // addrinfo
#include "network/get-sockaddr.h"       // Bytes, get_sockaddr()

Network::Host::Host(const addrinfo& t_addrinfo) :
    m_addr(get_sockaddr(t_addrinfo)),
    m_name(t_addrinfo.ai_canonname)
{
}

auto Network::Host::operator=(const addrinfo& t_addrinfo) -> Network::Host&
{
    m_addr = get_sockaddr(t_addrinfo);
    m_name = t_addrinfo.ai_canonname;
    return *this;
}

auto Network::Host::operator<(const Host& t_host) const -> bool
{
    return m_addr < t_host.m_addr;  // NOLINT
}

auto Network::Host::operator>(const Host& t_host) const -> bool
{
    return m_addr > t_host.m_addr;  // NOLINT
}

auto Network::Host::operator==(const Host& t_host) const -> bool
{
    return m_addr == t_host.m_addr;  // NOLINT
}

auto Network::Host::address() const -> Network::Bytes
{
    return m_addr;
}

auto Network::Host::canonical_name() const -> Network::Hostname
{
    return m_name;
}

auto Network::Host::get_sockaddr(const addrinfo& ai) -> Network::Bytes
{
    return (ai.ai_addr == nullptr ?
            Network::get_sockaddr() :
            Network::get_sockaddr(ai.ai_addr, ai.ai_addrlen));
}
