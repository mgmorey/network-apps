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
#include "network/os-features.h"        // WIN32

#ifdef WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

namespace Network
{
    struct Host
    {
        Host() noexcept = default;
        // cppcheck-suppress noExplicitConstructor
        Host(const addrinfo& t_addrinfo) noexcept;  // NOLINT
        Host(const Host&) noexcept = default;
        Host(Host&&) noexcept = default;
        ~Host() noexcept = default;
        auto operator=(const addrinfo& t_addrinfo) noexcept -> Host&;
        auto operator=(const Host&) noexcept -> Host& = default;
        auto operator=(Host&&) noexcept -> Host& = default;
        auto operator<(const Host& t_host) const noexcept -> bool;
        auto operator>(const Host& t_host) const noexcept -> bool;
        auto operator==(const Host& t_host) const noexcept -> bool;
        [[nodiscard]] auto address() const noexcept -> ByteString;
        [[nodiscard]] auto canonical_name() const noexcept -> OptionalHostname;

    protected:
        static auto to_canonical_name(const char* t_str) noexcept -> OptionalHostname;

    private:
        ByteString m_addr;
        OptionalHostname m_name;
    };
}

#endif
