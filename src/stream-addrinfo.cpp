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
#include "network/format.h"             // Format, operator<<()
#include "network/socketfamily.h"       // SocketFamily, operator<<()
#include "network/socketflags.h"        // SocketFlags, operator<<()
#include "network/socketprotocol.h"     // SocketProtocol,
                                        // operator<<()
#include "network/sockettype.h"         // SocketType, operator<<()
#include "network/string-null.h"        // string_null
#include "network/to-byte-string-sa.h"  // ByteString,
                                        // to_byte_string()
#include "network/to-size.h"            // to_size()
#include "network/to-string-bs.h"       // to_string()

#ifdef WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

#include <cstddef>      // std::size_t

auto Network::operator<<(std::ostream& os,
                         const addrinfo& ai) -> std::ostream&
{
    const SocketFlags flags(ai.ai_flags);
    const SocketFamily family(ai.ai_family);
    const SocketType socktype(ai.ai_socktype);
    const SocketProtocol protocol(family, ai.ai_protocol);

    if (ai.ai_addr == nullptr) {
        static constexpr auto delim {", "};
        static constexpr auto notab {0};
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
        static constexpr auto tab {9};
        const auto addr {to_byte_string(ai.ai_addr, to_size(ai.ai_addrlen))};
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
           << to_string(addr)
           << Format(tab, "ai_canonname")
           << (ai.ai_canonname == nullptr ? string_null : ai.ai_canonname)
           << Format(tab)
           << "...)";
    }

    return os;
}
