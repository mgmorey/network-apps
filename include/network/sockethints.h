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
#include "network/socketfamily.h"               // SocketFamily,
                                                // operator<<()
#include "network/socketflags.h"                // SocketFlags,
                                                // operator<<()
#include "network/socketprotocol.h"             // SocketProtocol,
                                                // operator<<()
#include "network/sockettype.h"                 // SocketType,
                                                // operator<<()

#ifdef WIN32
#include <ws2tcpip.h>       // AF_UNSPEC, addrinfo
#else
#include <netdb.h>          // addrinfo
#include <sys/socket.h>     // AF_UNSPEC
#endif

namespace Network
{
    struct SocketHints
    {
        friend auto operator<<(std::ostream& os,
                               const SocketHints& hints) noexcept -> std::ostream&;

        constexpr SocketHints() noexcept = default;

        constexpr explicit SocketHints(socket_flags_type t_flags,
                                       socket_family_type t_family = AF_UNSPEC,
                                       socket_type_type t_socktype = 0,
                                       socket_protocol_type t_protocol = 0) noexcept :
            m_flags(t_flags),
            m_family(t_family),
            m_socktype(t_socktype),
            m_protocol(t_protocol)
        {
        }

        // cppcheck-suppress noExplicitConstructor
        constexpr SocketHints(const addrinfo& t_addrinfo) noexcept :  // NOLINT
            m_flags(t_addrinfo.ai_flags),
            m_family(t_addrinfo.ai_family),
            m_socktype(t_addrinfo.ai_socktype),
            m_protocol(t_addrinfo.ai_protocol)
        {
        }

        constexpr SocketHints(const SocketHints&) noexcept = default;
        constexpr SocketHints(SocketHints&&) noexcept = default;
        constexpr ~SocketHints() noexcept = default;
        constexpr auto operator=(const SocketHints&) noexcept ->
            SocketHints& = default;
        constexpr auto operator=(SocketHints&&) noexcept ->
            SocketHints& = default;

        constexpr auto operator=(const addrinfo& t_addrinfo) noexcept ->
            SocketHints&
        {
            m_flags = t_addrinfo.ai_flags;
            m_family = t_addrinfo.ai_family;
            m_socktype = t_addrinfo.ai_socktype;
            m_protocol = t_addrinfo.ai_protocol;
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
            ai.ai_flags = m_flags;
            ai.ai_family = m_family;
            ai.ai_socktype = m_socktype;
            ai.ai_protocol = m_protocol;
            return ai;
        }

        [[nodiscard]] constexpr auto flags() const noexcept -> SocketFlags
        {
            return SocketFlags(m_flags);
        }

        [[nodiscard]] constexpr auto family() const noexcept -> SocketFamily
        {
            return SocketFamily(m_family);
        }

        [[nodiscard]] constexpr auto socktype() const noexcept -> SocketType
        {
            return SocketType(m_socktype);
        }

        [[nodiscard]] constexpr auto protocol() const noexcept -> SocketProtocol
        {
            return {m_family, m_protocol};
        }

    private:
        socket_flags_type m_flags {};
        socket_family_type m_family {};
        socket_type_type m_socktype {};
        socket_protocol_type m_protocol {};
    };

    extern auto operator<<(std::ostream& os,
                           const SocketHints& hints) noexcept -> std::ostream&;
}

#endif
