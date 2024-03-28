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
#include "network/get-sin6-addr.h"              // get_sin6_addr()
#include "network/get-sin6-port.h"              // get_sin6_port()
#include "network/port-type.h"                  // port_type
#include "network/to-string-in6-addr.h"         // to_string()

#ifdef WIN32
#include <winsock2.h>       // in6_addr
#else
#include <netinet/in.h>     // in6_addr
#endif

#include <string>       // std::string

auto Network::Address::sin6_addr() const -> in6_addr
{
    return get_sin6_addr(m_value);
}

auto Network::Address::sin6_port() const -> port_type
{
    return get_sin6_port(m_value);
}

auto Network::Address::sin6_text() const -> std::string
{
    return to_string(sin6_addr());
}
