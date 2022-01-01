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

#include "network/address.h"            // AF_INET, AF_INET6, AF_UNIX,
                                        // Address, htons(),
                                        // length_type, value_type
#include "network/get-sa-family.h"      // get_sa_family()
#include "network/get-sa-length.h"      // get_sa_length()
#include "network/get-sa-pointer.h"     // get_sa_pointer()
#include "network/offsets.h"            // sa_data_offset

#include <string>       // std::string
#include <utility>      // std::move

Network::Address::Address(value_type t_value) :
    m_value(std::move(t_value))
{
}

auto Network::Address::operator=(const value_type& t_value) -> Network::Address&
{
    m_value = t_value;
    return *this;
}

auto Network::Address::operator<(const Address& t_address) const -> bool
{
    return (family() < t_address.family() ||
            port() < t_address.port() ||
            text() < t_address.text());
}

auto Network::Address::operator>(const Address& t_address) const -> bool
{
    return (family() > t_address.family() ||
            port() > t_address.port() ||
            text() > t_address.text());
}

auto Network::Address::operator==(const Address& t_address) const -> bool
{
    return (family() == t_address.family() &&
            port() == t_address.port() &&
            text() == t_address.text());
}

Network::Address::operator value_type() const
{
    return m_value;
}

auto Network::Address::empty() const -> bool
{
    return m_value.empty();
}

auto Network::Address::family() const -> Network::family_type
{
    return sa_family();
}

auto Network::Address::length() const -> Network::Address::length_type
{
    return sa_length();
}

auto Network::Address::port() const -> Network::Address::port_type
{
    switch (sa_family()) {
    case AF_INET:
        return htons(sin_port());
    case AF_INET6:
        return htons(sin6_port());
    default:
        return 0;
    }
}

auto Network::Address::text() const -> std::string
{
    switch (sa_family()) {
#ifndef _WIN32
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
