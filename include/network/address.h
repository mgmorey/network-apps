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

#include "network/bytestring.h"                 // ByteString
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/port-type.h"                  // port_type
#include "network/socket-family-type.h"         // socket_family_type

#ifdef HAVE_SOCKADDR_SA_LEN
#include "network/socket-length-type.h"         // socket_length_type
#endif

#ifdef WIN32
#include <winsock2.h>       // in_addr
#include <ws2tcpip.h>       // in6_addr
#else
#include <netinet/in.h>     // in_addr, in6_addr
#endif

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class Address
    {
        friend auto operator<<(std::ostream& os,
                               const Address& address) ->
            std::ostream&;

    public:
        using value_type = ByteString;

        Address() = default;
        explicit Address(const value_type& t_value);
        explicit Address(value_type&& t_value);
        Address(const Address&) = default;
        Address(Address&&) = default;
        ~Address() = default;
        auto operator=(const Address&) -> Address& = default;
        auto operator=(Address&&) -> Address& = default;
        auto operator=(const value_type& t_value) -> Address&;
        auto operator<(const Address& t_address) const -> bool;
        auto operator>(const Address& t_address) const -> bool;
        auto operator==(const Address& t_address) const -> bool;
        explicit operator value_type() const;
        [[nodiscard]] auto empty() const -> bool;
        [[nodiscard]] auto family() const -> socket_family_type;
#ifdef HAVE_SOCKADDR_SA_LEN
        [[nodiscard]] auto length() const -> socket_length_type;
#endif
        [[nodiscard]] auto port() const -> port_type;
        [[nodiscard]] auto text() const -> std::string;

    protected:
        [[nodiscard]] auto sa_family() const -> socket_family_type;
#ifdef HAVE_SOCKADDR_SA_LEN
        [[nodiscard]] auto sa_length() const -> socket_length_type;
#endif
        [[nodiscard]] auto sa_text() const -> std::string;
        [[nodiscard]] auto sin_addr() const -> in_addr;
        [[nodiscard]] auto sin_port() const -> port_type;
        [[nodiscard]] auto sin_text() const -> std::string;
        [[nodiscard]] auto sin6_addr() const -> in6_addr;
        [[nodiscard]] auto sin6_port() const -> port_type;
        [[nodiscard]] auto sin6_text() const -> std::string;
#ifndef WIN32
        [[nodiscard]] auto sun_text() const -> std::string;
#endif

    private:
        value_type m_value;
    };

    extern auto operator<<(std::ostream& os,
                           const Address& address) -> std::ostream&;
}

#endif
