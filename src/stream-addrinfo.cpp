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

#include "network/addrinfo.h"           // operator<<(), std::ostream
#include "network/family.h"             // Family, operator<<()
#include "network/flags.h"              // Flags, operator<<()
#include "network/format.h"             // Format, operator<<()
#include "network/protocol.h"           // Protocol, operator<<()
#include "network/socktype.h"           // SockType, operator<<()
#include "network/string-null.h"        // string_null
#include "network/to-byte-string-sa.h"  // Bytes, to_byte_string()

#ifdef _WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

auto Network::operator<<(std::ostream& os,
                         const addrinfo& ai) -> std::ostream&
{
    constexpr auto tab {9};

    const Flags flags(ai.ai_flags);
    const Family family(ai.ai_family);
    const SockType socktype(ai.ai_socktype);
    const Protocol protocol(family, ai.ai_protocol);

    os << "addrinfo("
       << Format("ai_flags")
       << flags
       << Format(tab, "ai_family")
       << family
       << Format(tab, "ai_socktype")
       << socktype
       << Format(tab, "ai_protocol")
       << protocol
       << Format(tab, "ai_addrlen")
       << ai.ai_addrlen
       << Format(tab, "ai_addr")
       << to_byte_string(ai.ai_addr, ai.ai_addrlen)
       << Format(tab, "ai_canonname")
       << (ai.ai_canonname == nullptr ? string_null : ai.ai_canonname)
       << Format(tab)
       << "...)";
    return os;
}
