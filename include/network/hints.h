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

#ifndef NETWORK_HINTS_H
#define NETWORK_HINTS_H

#include "network/family.h"             // Family, operator<<()
#include "network/flags.h"              // Flags, operator<<()
#include "network/os-features.h"        // WIN32
#include "network/protocol.h"           // Protocol, operator<<()
#include "network/socktype.h"           // SockType, operator<<()

#ifdef WIN32
#include <ws2tcpip.h>       // AF_UNSPEC, addrinfo
#else
#include <netdb.h>          // addrinfo
#include <sys/socket.h>     // AF_UNSPEC
#endif

namespace Network
{
    struct Hints
    {
        constexpr Hints() noexcept = default;

        constexpr explicit Hints(flags_type t_flags,
                                 family_type t_family = AF_UNSPEC,
                                 socktype_type t_socktype = 0,
                                 protocol_type t_protocol = 0) noexcept :
            m_flags(t_flags),
            m_family(t_family),
            m_socktype(t_socktype),
            m_protocol(t_protocol)
        {
        }

        // cppcheck-suppress noExplicitConstructor
        constexpr Hints(const addrinfo& t_addrinfo) noexcept :  // NOLINT
            m_flags(t_addrinfo.ai_flags),
            m_family(t_addrinfo.ai_family),
            m_socktype(t_addrinfo.ai_socktype),
            m_protocol(t_addrinfo.ai_protocol)
        {
        }

        constexpr Hints(const Hints&) noexcept = default;
        constexpr Hints(Hints&&) noexcept = default;
        constexpr ~Hints() noexcept = default;
        constexpr auto operator=(const Hints&) noexcept -> Hints& = default;
        constexpr auto operator=(Hints&&) noexcept -> Hints& = default;

        constexpr auto operator=(const addrinfo& t_addrinfo) noexcept -> Hints&
        {
            m_flags = t_addrinfo.ai_flags;
            m_family = t_addrinfo.ai_family;
            m_socktype = t_addrinfo.ai_socktype;
            m_protocol = t_addrinfo.ai_protocol;
            return *this;
        }

        constexpr auto operator<(const Hints& t_hints) const noexcept -> bool
        {
            return (m_protocol < t_hints.m_protocol ||
                    m_socktype < t_hints.m_socktype ||
                    m_family < t_hints.m_family);
        }

        constexpr auto operator>(const Hints& t_hints) const noexcept -> bool
        {
            return (m_protocol > t_hints.m_protocol ||
                    m_socktype > t_hints.m_socktype ||
                    m_family > t_hints.m_family);
        }

        constexpr auto operator==(const Hints& t_hints) const noexcept -> bool
        {
            return (m_protocol == t_hints.m_protocol &&
                    m_socktype == t_hints.m_socktype &&
                    m_family == t_hints.m_family);
        }

        constexpr operator addrinfo() const noexcept  // NOLINT
        {
            addrinfo ai {};
            ai.ai_flags = static_cast<int>(m_flags);
            ai.ai_family = static_cast<int>(m_family);
            ai.ai_socktype = static_cast<int>(m_socktype);
            ai.ai_protocol = static_cast<int>(m_protocol);
            return ai;
        }

        [[nodiscard]] constexpr auto flags() const noexcept -> Flags
        {
            return Flags(m_flags);
        }

        [[nodiscard]] constexpr auto family() const noexcept -> Family
        {
            return Family(m_family);
        }

        [[nodiscard]] constexpr auto socktype() const noexcept -> SockType
        {
            return SockType(m_socktype);
        }

        [[nodiscard]] constexpr auto protocol() const noexcept -> Protocol
        {
            return {m_family, m_protocol};
        }

    private:
        flags_type m_flags {};
        family_type m_family {};
        socktype_type m_socktype {};
        protocol_type m_protocol {};
    };

    extern auto operator<<(std::ostream& os,
                           const Hints& hints) -> std::ostream&;
}

#endif
