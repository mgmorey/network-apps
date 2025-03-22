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

#include "network/bytestring.h"         // ByteString
#include "network/family-type.h"        // family_type
#include "network/os-features.h"        // HAVE_SOCKADDR_SA_LEN
#include "network/port-type.h"          // port_type

#ifdef HAVE_SOCKADDR_SA_LEN
#include "network/socket-length-type.h" // socket_length_type
#endif

#ifdef WIN32
#include <winsock2.h>       // in_addr
#include <ws2tcpip.h>       // in6_addr
#else
#include <netinet/in.h>     // in_addr, in6_addr
#endif

#include <cstddef>      // std::byte
#include <ostream>      // std::ostream
#include <span>         // std::span
#include <string>       // std::string
#ifndef WIN32
#include <string_view> // std::string_view
#endif

namespace Network
{
    class Address
    {
        friend auto operator<<(std::ostream& os,
                               const Address& address) ->
            std::ostream&;

    public:
        using address_type = ByteString;

        explicit Address(std::span<const std::byte> t_bs);

        Address() = default;
        Address(const Address&) = default;
        Address(Address&&) = default;
        ~Address() = default;
        auto operator=(const Address&) -> Address& = default;
        auto operator=(Address&&) -> Address& = default;

        auto operator=(std::span<const std::byte> t_bs) -> Address&;
        explicit operator address_type() const;

        [[nodiscard]] auto empty() const -> bool;
        [[nodiscard]] auto family() const -> family_type;
#ifdef HAVE_SOCKADDR_SA_LEN
        [[nodiscard]] auto length() const -> socket_length_type;
#endif
        [[nodiscard]] auto port() const -> port_type;
        [[nodiscard]] auto text() const -> std::string;
        [[nodiscard]] auto value() const -> address_type;

    protected:
        [[nodiscard]] auto sa_family() const -> family_type;
#ifdef HAVE_SOCKADDR_SA_LEN
        [[nodiscard]] auto sa_length() const -> socket_length_type;
#endif
        [[nodiscard]] auto sin_addr() const -> in_addr;
        [[nodiscard]] auto sin_port() const -> port_type;
        [[nodiscard]] auto sin_text() const -> std::string;
        [[nodiscard]] auto sin6_addr() const -> in6_addr;
        [[nodiscard]] auto sin6_port() const -> port_type;
        [[nodiscard]] auto sin6_text() const -> std::string;
#ifndef WIN32
        [[nodiscard]] auto sun_text() const -> std::string_view;
#endif

    private:
        address_type m_addr;
        std::span<const std::byte> m_span;
    };

    extern auto operator<<(std::ostream& os,
                           const Address& address) -> std::ostream&;
}

#endif
