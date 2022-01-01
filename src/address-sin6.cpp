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

#include "network/address.h"            // AF_INET6, INET6_ADDRSTRLEN,
                                        // Address, in6_addr
                                        // inet_ntop(), port_type,
                                        // sockaddr_in6
#include "network/buffer.h"             // Buffer
#include "network/get-sin6-pointer.h"   // get_sin6_pointer()

#include <string>       // std::string

auto Network::Address::sin6() const -> const sockaddr_in6*
{
    return get_sin6_pointer(m_value);
}

auto Network::Address::sin6_addr() const -> in6_addr
{
    return sin6()->sin6_addr;
}

auto Network::Address::sin6_port() const -> Network::Address::port_type
{
    return sin6()->sin6_port;
}

auto Network::Address::sin6_text() const -> std::string
{
    Buffer buffer {INET6_ADDRSTRLEN};
    const auto in6 {sin6_addr()};
    ::inet_ntop(AF_INET6, &in6, buffer.data(), buffer.size());
    return std::string(buffer);
}
