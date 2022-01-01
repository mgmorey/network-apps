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

#ifndef NETWORK_HOST_H
#define NETWORK_HOST_H

#include "network/bytes.h"              // Bytes
#include "network/hostname.h"           // Hostname

#ifdef _WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

namespace Network
{
    struct Host
    {
        static auto get_sockaddr(const addrinfo& ai) -> Bytes;

        Host() = default;
        // cppcheck-suppress noExplicitConstructor
        Host(const addrinfo& t_addrinfo);  // NOLINT
        auto operator=(const addrinfo& t_addrinfo) -> Host&;
        auto operator<(const Host& t_host) const -> bool;
        auto operator>(const Host& t_host) const -> bool;
        auto operator==(const Host& t_host) const -> bool;
        [[nodiscard]] auto address() const -> Bytes;
        [[nodiscard]] auto canonical_name() const -> Hostname;

    private:
        Bytes m_sock_addr;
        Hostname m_canonname;
    };
}

#endif
