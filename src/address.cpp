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

#include "network/address.h"            // Address
#include "network/family-type.h"        // family_type
#include "network/familyerror.h"        // FamilyError
#include "network/os-features.h"        // HAVE_SOCKADDR_SA_LEN
#include "network/port-type.h"          // port_type
#include "network/quote.h"              // quote()

#ifdef HAVE_SOCKADDR_SA_LEN
#include "network/socket-length-type.h" // socket_length_type
#endif

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNIX
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNIX
#endif

#include <cstddef>      // std::byte
#include <span>         // std::span
#include <string>       // std::string

Network::Address::Address(std::span<const std::byte> t_bs) :
    m_addr(t_bs.begin(), t_bs.end()),
    m_span(m_addr)
{
}

auto Network::Address::operator=(std::span<const std::byte> t_bs) -> Address&
{
    m_addr.assign(t_bs.begin(), t_bs.end());
    m_span = m_addr;
    return *this;
}

Network::Address::operator address_type() const
{
    return m_addr;
}

auto Network::Address::empty() const -> bool
{
    return m_addr.empty();
}

auto Network::Address::family() const -> family_type
{
    return sa_family();
}

#ifdef HAVE_SOCKADDR_SA_LEN

auto Network::Address::length() const -> socket_length_type
{
    return sa_length();
}

#endif

auto Network::Address::port() const -> port_type
{
    switch (sa_family()) {
    case AF_INET:
        return sin_port();
    case AF_INET6:
        return sin6_port();
    default:
        throw FamilyError(sa_family());
    }
}

auto Network::Address::text() const -> std::string
{
    switch (sa_family()) {
    case AF_INET:
        return sin_text();
    case AF_INET6:
        return sin6_text();
#ifndef WIN32
    case AF_UNIX:
        return quote(sun_text());
#endif
    default:
        throw FamilyError(sa_family());
    }
}
