// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/stream-addrinfo.hpp"  // operator<<()
#include "network/format.hpp"           // Format
#include "network/socketfamily.hpp"     // SocketFamily,
#include "network/socketflags.hpp"      // SocketFlags
#include "network/socketprotocol.hpp"   // SocketProtocol
#include "network/sockettype.hpp"       // SocketType
#include "network/string-null.hpp"      // string_null
#include "network/to-sa-length.hpp"     // to_sa_length()
#include "network/to-string-void.hpp"   // to_string()

#ifdef _WIN32
#include <ws2tcpip.h>       // addrinfo
#else
#include <netdb.h>          // addrinfo
#endif

#include <ostream>      // std::ostream

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
           << to_string(ai.ai_addr, to_sa_length(ai.ai_addrlen))
           << Format(tab, "ai_canonname")
           << (ai.ai_canonname == nullptr ? string_null : ai.ai_canonname)
           << Format(tab)
           << "...)";
    }

    return os;
}
