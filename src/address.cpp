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

#include "network/address.h"                    // Address
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/port-type.h"                  // port_type
#include "network/socket-family-type.h"         // socket_family_type
#include "network/validate-bs.h"                // validate()

#ifdef HAVE_SOCKADDR_SA_LEN
#include "network/socket-length-type.h"         // socket_length_type
#endif

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6
#else
#include <sys/socket.h>     // AF_INET, AF_INET6
#endif

#include <string>       // std::string
#include <utility>      // std::move()

Network::Address::Address(const value_type& t_value) :
    m_value(t_value)
{
    validate(m_value);
}

Network::Address::Address(value_type&& t_value) :
    m_value(std::move(t_value))
{
    validate(m_value);
}

auto Network::Address::operator=(const value_type& t_value) -> Address&
{
    validate(t_value);
    m_value = t_value;
    return *this;
}

Network::Address::operator value_type() const
{
    return m_value;
}

auto Network::Address::empty() const -> bool
{
    return m_value.empty();
}

auto Network::Address::family() const -> socket_family_type
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
        return 0;
    }
}

auto Network::Address::text() const -> std::string
{
    switch (sa_family()) {
#ifndef WIN32
    case AF_UNIX:
        return sun_text();
#endif
    case AF_INET:
        return sin_text();
    case AF_INET6:
        return sin6_text();
    default:
        return sa_text();
    }
}
