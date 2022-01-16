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

#include "network/bytestring.h"         // ByteString
#include "network/optionalhostname.h"   // OptionalHostname

#ifdef _WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

namespace Network
{
    struct Host
    {
        Host() = default;
        // cppcheck-suppress noExplicitConstructor
        Host(const addrinfo& t_addrinfo);  // NOLINT
        Host(const Host&) = default;
        Host(Host&&) noexcept = default;
        ~Host() = default;
        auto operator=(const addrinfo& t_addrinfo) -> Host&;
        auto operator=(const Host&) -> Host& = default;
        auto operator=(Host&&) noexcept -> Host& = default;
        auto operator<(const Host& t_host) const -> bool;
        auto operator>(const Host& t_host) const -> bool;
        auto operator==(const Host& t_host) const -> bool;
        [[nodiscard]] auto address() const -> ByteString;
        [[nodiscard]] auto canonical_name() const -> OptionalHostname;

    protected:
        static auto to_canonical_name(const char* t_str) -> OptionalHostname;

    private:
        ByteString m_addr;
        OptionalHostname m_name;
    };
}

#endif
