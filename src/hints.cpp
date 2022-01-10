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

#include "network/hints.h"              // Hints, addrinfo

Network::Hints::Hints(family_type t_family,
                      socktype_type t_socktype,
                      protocol_type t_protocol,
                      flags_type t_flags) :
    m_flags(t_flags),
    m_family(t_family),
    m_socktype(t_socktype),
    m_protocol(t_protocol)
{
}

Network::Hints::Hints(const addrinfo& t_addrinfo) :
    m_flags(t_addrinfo.ai_flags),
    m_family(t_addrinfo.ai_family),
    m_socktype(t_addrinfo.ai_socktype),
    m_protocol(t_addrinfo.ai_protocol)
{
}

auto Network::Hints::operator=(const addrinfo& t_addrinfo) -> Network::Hints&
{
    m_flags = t_addrinfo.ai_flags;
    m_family = t_addrinfo.ai_family;
    m_socktype = t_addrinfo.ai_socktype;
    m_protocol = t_addrinfo.ai_protocol;
    return *this;
}

auto Network::Hints::operator<(const Hints& t_hints) const -> bool
{
    return (m_protocol < t_hints.m_protocol ||
            m_socktype < t_hints.m_socktype ||
            m_family < t_hints.m_family);
}

auto Network::Hints::operator>(const Hints& t_hints) const -> bool
{
    return (m_protocol > t_hints.m_protocol ||
            m_socktype > t_hints.m_socktype ||
            m_family > t_hints.m_family);
}

auto Network::Hints::operator==(const Hints& t_hints) const -> bool
{
    return (m_protocol == t_hints.m_protocol &&
            m_socktype == t_hints.m_socktype &&
            m_family == t_hints.m_family);
}

Network::Hints::operator addrinfo() const
{
    addrinfo ai {};
    ai.ai_flags = static_cast<int>(m_flags);
    ai.ai_family = static_cast<int>(m_family);
    ai.ai_socktype = static_cast<int>(m_socktype);
    ai.ai_protocol = static_cast<int>(m_protocol);
    return ai;
}

auto Network::Hints::flags() const -> Network::Flags
{
    return Flags(m_flags);
}

auto Network::Hints::family() const -> Network::Family
{
    return Family(m_family);
}

auto Network::Hints::socktype() const -> Network::SockType
{
    return SockType(m_socktype);
}

auto Network::Hints::protocol() const -> Network::Protocol
{
    return {m_family, m_protocol};
}
