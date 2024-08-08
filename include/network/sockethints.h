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

#ifndef NETWORK_SOCKETHINTS_H
#define NETWORK_SOCKETHINTS_H

#include "network/socket-family-type.h"         // socket_family_type
#include "network/socket-flags-type.h"          // socket_flags_type
#include "network/socket-protocol-type.h"       // socket_protocol_type
#include "network/socket-type-type.h"           // socket_type_type

#ifdef WIN32
#include <ws2tcpip.h>       // addrinfo
#else
#include <netdb.h>          // addrinfo
#endif

#include <ostream>      // std::ostream

namespace Network
{
    struct SocketHints
    {
        constexpr SocketHints() noexcept = default;

        constexpr explicit SocketHints(socket_family_type t_family,
                                       socket_type_type t_socktype,
                                       socket_protocol_type t_protocol = 0,
                                       socket_flags_type t_flags = 0) noexcept :
            m_family(t_family),
            m_socktype(t_socktype),
            m_protocol(t_protocol),
            m_flags(t_flags)
        {
        }

        // cppcheck-suppress noExplicitConstructor; NOLINTNEXTLINE
        constexpr SocketHints(const addrinfo& t_ai) noexcept :
            m_family(t_ai.ai_family),
            m_socktype(t_ai.ai_socktype),
            m_protocol(t_ai.ai_protocol),
            m_flags(t_ai.ai_flags)
        {
        }

        constexpr SocketHints(const SocketHints&) noexcept = default;
        constexpr SocketHints(SocketHints&&) noexcept = default;
        constexpr ~SocketHints() noexcept = default;
        constexpr auto operator=(const SocketHints&) noexcept ->
            SocketHints& = default;
        constexpr auto operator=(SocketHints&&) noexcept ->
            SocketHints& = default;

        constexpr auto operator=(const addrinfo& t_ai) noexcept ->
            SocketHints&
        {
            m_family = t_ai.ai_family;
            m_socktype = t_ai.ai_socktype;
            m_protocol = t_ai.ai_protocol;
            m_flags = t_ai.ai_flags;
            return *this;
        }

        constexpr auto operator<(const SocketHints& t_hints) const noexcept ->
            bool
        {
            return (m_protocol < t_hints.m_protocol ||
                    m_socktype < t_hints.m_socktype ||
                    m_family < t_hints.m_family);
        }

        constexpr auto operator>(const SocketHints& t_hints) const noexcept ->
            bool
        {
            return (m_protocol > t_hints.m_protocol ||
                    m_socktype > t_hints.m_socktype ||
                    m_family > t_hints.m_family);
        }

        constexpr auto operator==(const SocketHints& t_hints) const noexcept ->
            bool
        {
            return (m_protocol == t_hints.m_protocol &&
                    m_socktype == t_hints.m_socktype &&
                    m_family == t_hints.m_family);
        }

        constexpr operator addrinfo() const noexcept  // NOLINT
        {
            addrinfo ai {};
            ai.ai_family = m_family;
            ai.ai_socktype = m_socktype;
            ai.ai_protocol = m_protocol;
            ai.ai_flags = m_flags;
            return ai;
        }

        socket_family_type m_family {};  // NOLINT
        socket_type_type m_socktype {};  // NOLINT
        socket_protocol_type m_protocol {};  // NOLINT
        socket_flags_type m_flags {};  // NOLINT
    };

    extern auto operator<<(std::ostream& os,
                           const SocketHints& hints) noexcept -> std::ostream&;
}

#endif
