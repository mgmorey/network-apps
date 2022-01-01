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

#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include "network/hints.h"          // Hints
#include "network/host.h"           // Host

#ifdef _WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

namespace Network
{
    struct Socket :
        public Hints,
        public Host
    {
        explicit Socket(int t_family = 0,
                        int t_socktype = 0,
                        int t_protocol = 0,
                        int t_flags = 0);
        // cppcheck-suppress noExplicitConstructor
        Socket(const addrinfo& t_addrinfo);  // NOLINT
        auto operator=(const addrinfo& t_addrinfo) -> Socket&;
        auto operator<(const Socket& t_sock) const -> bool;
        auto operator>(const Socket& t_sock) const -> bool;
        auto operator==(const Socket& t_sock) const -> bool;
    };

    extern auto operator<<(std::ostream& os,
                           const Socket& sock) -> std::ostream&;
}

#endif
