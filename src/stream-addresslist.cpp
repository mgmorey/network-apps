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

#include "network/addresslist.h"                // AddressList,
                                                // operator<<()
#include "network/format.h"                     // Format
#include "network/socketfamily.h"               // SocketFamily,
#include "network/socketflags.h"                // SocketFlags
#include "network/sockethost.h"                 // SocketHost
#include "network/socketprotocol.h"             // SocketProtocol
#include "network/sockettype.h"                 // SocketType
#include "network/string-null.h"                // string_null
#include "network/to-string-vector-byte.h"      // to_string()

#ifdef WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

#include <ostream>      // std::ostream


auto Network::operator<<(std::ostream& os, const AddressList& list) -> std::ostream&
{
    for (const auto& node : list) {
        os << node
           << std::endl;
    }

    return os;
}

auto Network::operator<<(std::ostream& os,
                         const addrinfo& ai) -> std::ostream&
{
    const SocketFlags flags {ai.ai_flags};
    const SocketFamily family {ai.ai_family};
    const SocketType socktype {ai.ai_socktype};
    const SocketProtocol protocol {ai.ai_protocol, family};

    if (ai.ai_addr == nullptr) {
        constexpr auto delim {", "};
        constexpr auto notab {0};
        os << "addrinfo("
           << Format("ai_flags")
           << flags
           << Format(delim, notab, "ai_family")
           << family
           << Format(delim, notab, "ai_socktype")
           << socktype
           << Format(delim, notab, "ai_protocol")
           << protocol
           << Format(delim, notab)
           << "...)";
    }
    else {
        constexpr auto tab {9};
        const SocketHost host {ai};
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
           << to_string(host.address())
           << Format(tab, "ai_canonname")
           << host.canonical_name().value_or(string_null)
           << Format(tab)
           << "...)";
    }

    return os;
}
