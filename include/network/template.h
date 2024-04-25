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

#ifndef NETWORK_TEMPLATE_H
#define NETWORK_TEMPLATE_H

#include "network/sockethints.h"        // SocketHints
#include "network/sockethost.h"         // SocketHost

#ifdef WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

namespace Network
{
    struct Template
    {
        friend auto operator<<(std::ostream& os,
                               const Template& sock) noexcept -> std::ostream&;

        Template() noexcept = default;
        // cppcheck-suppress noExplicitConstructor
        Template(const addrinfo& t_ai) noexcept;  // NOLINT
        Template(const Template&) noexcept = default;
        Template(Template&&) = default;
        ~Template() noexcept = default;
        auto operator=(const Template&) noexcept -> Template& = default;
        auto operator=(Template&&) noexcept -> Template& = default;
        auto operator=(const addrinfo& t_ai) noexcept -> Template&;
        auto operator<(const Template& t_sock) const noexcept -> bool;
        auto operator>(const Template& t_sock) const noexcept -> bool;
        auto operator==(const Template& t_sock) const noexcept -> bool;
        [[nodiscard]] auto hints() const noexcept -> const SocketHints&;
        [[nodiscard]] auto host() const noexcept -> const SocketHost&;
        [[nodiscard]] auto address() const noexcept -> const ByteString&;
        [[nodiscard]] auto canonical_name() const noexcept ->
            const OptionalHostname&;

    private:
        SocketHints m_hints;
        SocketHost m_host;
    };

    extern auto operator<<(std::ostream& os,
                           const Template& sock) noexcept -> std::ostream&;
}

#endif
