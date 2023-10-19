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

#include "network/address.h"                    // Address,
                                                // value_type
#include "network/addresserror.h"               // AddressError
#include "network/is-valid.h"                   // is_valid()
#include "network/length-type.h"                // length_type
#include "network/port-type.h"                  // port_type
#include "network/socket-family-type.h"         // socket_family_type

#include <cstddef>                              // std::size_t

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
    if (!is_valid(m_value)) {
        throw AddressError(m_value);
    }
}

Network::Address::Address(value_type&& t_value) :
    m_value(std::move(t_value))
{
    if (!is_valid(m_value)) {
        throw AddressError(m_value);
    }
}

auto Network::Address::operator=(const value_type& t_value) ->
    Network::Address&
{
    if (!is_valid(t_value)) {
        throw AddressError(t_value);
    }

    m_value = t_value;
    return *this;
}

auto Network::Address::operator<(const Address& t_address) const noexcept ->
    bool
{
    return (family() < t_address.family() ||
            port() < t_address.port() ||
            text() < t_address.text());
}

auto Network::Address::operator>(const Address& t_address) const noexcept ->
    bool
{
    return (family() > t_address.family() ||
            port() > t_address.port() ||
            text() > t_address.text());
}

auto Network::Address::operator==(const Address& t_address) const noexcept ->
    bool
{
    return (family() == t_address.family() &&
            port() == t_address.port() &&
            text() == t_address.text());
}

Network::Address::operator value_type() const
{
    return m_value;
}

auto Network::Address::data() const noexcept -> Network::Address::value_type
{
    return sa_data();
}

auto Network::Address::empty() const noexcept -> bool
{
    return m_value.empty();
}

auto Network::Address::family() const noexcept -> Network::socket_family_type
{
    return sa_family();
}

auto Network::Address::length() const noexcept -> Network::length_type
{
    return sa_length();
}

auto Network::Address::port() const noexcept -> Network::port_type
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

auto Network::Address::size() const noexcept -> std::size_t
{
    return m_value.size();
}

auto Network::Address::text() const noexcept -> std::string
{
    switch (sa_family()) {
    case AF_UNIX:
        return sun_text();
    case AF_INET:
        return sin_text();
    case AF_INET6:
        return sin6_text();
    default:
        return sa_text();
    }
}
