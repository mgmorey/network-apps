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

#ifndef NETWORK_ADDRESS_H
#define NETWORK_ADDRESS_H

#include "network/bytes.h"              // Bytes, operator<<()
#include "network/family-type.h"        // family_type
#include "network/length-type.h"        // length_type
#include "network/port-type.h"          // port_type

#ifdef _WIN32
#include <winsock2.h>       // sockaddr, sockaddr_in
#include <ws2tcpip.h>       // inet_ntop(), sockaddr_in6
#else
#include <arpa/inet.h>      // inet_ntop()
#include <netinet/in.h>     // sockaddr_in, sockaddr_in6
#include <sys/socket.h>     // sockaddr
#include <sys/un.h>         // sockaddr_un
#endif

#include <cstddef>      // offsetof()
#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class Address
    {
    public:
        using value_type = Bytes;

        // cppcheck-suppress noExplicitConstructor
        Address(value_type t_value);  // NOLINT
        auto operator=(const value_type& t_value) -> Address&;
        auto operator<(const Address& t_address) const -> bool;
        auto operator>(const Address& t_address) const -> bool;
        auto operator==(const Address& t_address) const -> bool;
        operator value_type() const;  // NOLINT
        [[nodiscard]] auto empty() const -> bool;
        [[nodiscard]] auto family() const -> family_type;
        [[nodiscard]] auto length() const -> length_type;
        [[nodiscard]] auto port() const -> port_type;
        [[nodiscard]] auto text() const -> std::string;

    protected:
        [[nodiscard]] auto sa_data() const -> value_type;
        [[nodiscard]] auto sa_family() const -> family_type;
        [[nodiscard]] auto sa_length() const -> length_type;
        [[nodiscard]] auto sa_text() const -> std::string;

        [[nodiscard]] auto sin_addr() const -> in_addr;
        [[nodiscard]] auto sin_port() const -> port_type;
        [[nodiscard]] auto sin_text() const -> std::string;

        [[nodiscard]] auto sin6_addr() const -> in6_addr;
        [[nodiscard]] auto sin6_port() const -> port_type;
        [[nodiscard]] auto sin6_text() const -> std::string;

#ifndef _WIN32
        [[nodiscard]] auto sun_text() const -> std::string;
#endif

    private:
        value_type m_value;
    };

    extern auto operator<<(std::ostream& os,
                           const Address& address) -> std::ostream&;
}

#endif
