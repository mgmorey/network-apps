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
#include "network/protocol.h"           // Protocol, operator<<()
#include "network/socktype.h"           // SockType, operator<<()

#ifdef _WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

namespace Network
{
    struct Hints
    {
        Hints() = default;
        explicit Hints(family_type t_family,
                       socktype_type t_socktype = 0,
                       protocol_type t_protocol = 0,
                       flags_type t_flags = 0);
        // cppcheck-suppress noExplicitConstructor
        Hints(const addrinfo& t_addrinfo);  // NOLINT
        Hints(const Hints&) = default;
        Hints(Hints&&) noexcept = default;
        ~Hints() = default;
        auto operator=(const Hints&) -> Hints& = default;
        auto operator=(Hints&&) noexcept -> Hints& = default;
        auto operator=(const addrinfo& t_addrinfo) -> Hints&;
        auto operator<(const Hints& t_hints) const -> bool;
        auto operator>(const Hints& t_hints) const -> bool;
        auto operator==(const Hints& t_hints) const -> bool;
        operator addrinfo() const;  // NOLINT
        [[nodiscard]] auto flags() const -> Flags;
        [[nodiscard]] auto family() const -> Family;
        [[nodiscard]] auto socktype() const -> SockType;
        [[nodiscard]] auto protocol() const -> Protocol;

    private:
        flags_type m_flags {};
        family_type m_family {};
        socktype_type m_socktype {};
        protocol_type m_protocol {};
    };

    extern auto operator<<(std::ostream& os,
                           const Hints& hints) ->  std::ostream&;
}

#endif
