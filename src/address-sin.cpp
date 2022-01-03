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

#include "network/address.h"            // AF_INET, INET_ADDRSTRLEN,
                                        // Address, in_addr
                                        // inet_ntop(), port_type,
                                        // sockaddr_in
#include "network/buffer.h"             // Buffer
#include "network/get-sin-addr.h"       // get_sin_addr()
#include "network/get-sin-pointer.h"    // get_sin_pointer()
#include "network/get-sin-port.h"       // get_sin_port()

#include <string>       // std::string

auto Network::Address::sin() const -> const sockaddr_in*
{
    return get_sin_pointer(m_value);
}

auto Network::Address::sin_addr() const -> in_addr
{
    return get_sin_addr(m_value);
}

auto Network::Address::sin_port() const -> Network::port_type
{
    return get_sin_port(m_value);
}

auto Network::Address::sin_text() const -> std::string
{
    Buffer buffer {INET_ADDRSTRLEN};
    const auto in {sin_addr()};
    ::inet_ntop(AF_INET, &in, buffer.data(), buffer.size());
    return std::string(buffer);
}
