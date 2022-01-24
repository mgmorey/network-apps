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

#include "network/socket.h"             // Hints, Host, Socket
#include "network/os-features.h"        // WIN32

#ifdef WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

Network::Socket::Socket(int t_family,
                        int t_socktype,
                        int t_protocol,
                        int t_flags) :
    Hints(t_family, t_socktype, t_protocol, t_flags)
{
}

Network::Socket::Socket(const addrinfo& t_addrinfo) :
    Hints(t_addrinfo),
    Host(t_addrinfo)
{
}

auto Network::Socket::operator=(const addrinfo& t_addrinfo) -> Network::Socket&
{
    static_cast<Hints>(*this) = t_addrinfo;  // NOLINT
    static_cast<Host>(*this) = t_addrinfo;  // NOLINT
    return *this;
}

auto Network::Socket::operator<(const Socket& t_sock) const -> bool
{
    return (static_cast<Hints>(*this) < t_sock ||  // NOLINT
            static_cast<Host>(*this) < t_sock);  // NOLINT
}

auto Network::Socket::operator>(const Socket& t_sock) const -> bool
{
    return (static_cast<Hints>(*this) > t_sock ||  // NOLINT
            static_cast<Host>(*this) > t_sock);  // NOLINT
}

auto Network::Socket::operator==(const Socket& t_sock) const -> bool
{
    return (static_cast<Hints>(*this) == t_sock &&  // NOLINT
            static_cast<Host>(*this) == t_sock);  // NOLINT
}
