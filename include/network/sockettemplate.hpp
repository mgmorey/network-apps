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

#ifndef NETWORK_SOCKETTEMPLATE_HPP
#define NETWORK_SOCKETTEMPLATE_HPP

#include "network/bytespan.hpp"         // ByteSpan
#include "network/sockethints.hpp"      // SocketHints
#include "network/sockethost.hpp"       // SocketHost

#ifdef WIN32
#include <ws2tcpip.h>       // addrinfo
#else
#include <netdb.h>          // addrinfo
#endif

namespace Network
{
    struct SocketTemplate
    {
        friend auto operator<<(std::ostream& os,
                               const SocketTemplate& sock) -> std::ostream&;

        // cppcheck-suppress noExplicitConstructor; NOLINTNEXTLINE
        SocketTemplate(const addrinfo& t_ai) noexcept :
            m_hints(t_ai),
            m_host(t_ai)
        {
        }

        SocketTemplate() noexcept = default;
        SocketTemplate(const SocketTemplate&) noexcept = default;
        SocketTemplate(SocketTemplate&&) = default;
        ~SocketTemplate() noexcept = default;
        auto operator=(const SocketTemplate&) noexcept ->
            SocketTemplate& = default;
        auto operator=(SocketTemplate&&) noexcept ->
            SocketTemplate& = default;

        [[nodiscard]] auto hints() const noexcept -> const SocketHints&
        {
            return m_hints;
        }

        [[nodiscard]] auto address() const noexcept -> ByteSpan
        {
            return m_host.address();
        }

    private:
        SocketHints m_hints;
        SocketHost m_host;
    };
}

#endif
