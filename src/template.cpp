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

#include "network/template.hpp"         // Template
#include "network/bytespan.hpp"         // ByteSpan
#include "network/sockethints.hpp"      // SocketHints

#ifdef WIN32
#include <ws2tcpip.h>       // addrinfo
#else
#include <netdb.h>          // addrinfo
#endif

Network::Template::Template(const addrinfo& t_ai) noexcept :
    m_hints(t_ai),
    m_host(t_ai)
{
}

auto Network::Template::hints() const noexcept -> const SocketHints&
{
    return m_hints;
}

auto Network::Template::address() const noexcept -> ByteSpan
{
    return m_host.address();
}
