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
#include "network/get-sin6-addr.h"      // get_sin6_addr()
#include "network/get-sin6-pointer.h"   // get_sin6_pointer()
#include "network/get-sin6-port.h"      // get_sin6_port()
#include "network/to-string-sin6.h"     // std::string, to_string()

auto Network::Address::sin6_addr() const -> in6_addr
{
    return get_sin6_addr(m_value);
}

auto Network::Address::sin6_port() const -> Network::port_type
{
    return get_sin6_port(m_value);
}

auto Network::Address::sin6_text() const -> std::string
{
    const auto addr {sin6_addr()};
    return to_string(addr);
}
