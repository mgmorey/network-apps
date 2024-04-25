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

#include "network/format.h"                     // Format
#include "network/socketfamily.h"               // SocketFamily
#include "network/socketflags.h"                // SocketFlags
#include "network/sockethints.h"                // SocketHints
#include "network/socketprotocol.h"             // SocketProtocol
#include "network/sockettype.h"                 // SocketType
#include "network/string-null.h"                // string_null
#include "network/template.h"                   // Template
#include "network/to-string-vector-byte.h"      // to_string()

#include <ostream>      // operator<<(), std::ostream

auto Network::operator<<(std::ostream& os,
                         const Template& sock) noexcept -> std::ostream&
{
    static constexpr auto tab {9};
    const auto hints {sock.hints()};

    os << "Template("
       << Format("flags")
       << SocketFlags(hints.m_flags)
       << Format(tab, "family")
       << SocketFamily(hints.m_family)
       << Format(tab, "socktype")
       << SocketType(hints.m_socktype)
       << Format(tab, "protocol")
       << SocketProtocol(hints.m_protocol, hints.m_family)
       << Format(tab, "address")
       << to_string(sock.address())
       << Format(tab, "canonical_name")
       << sock.canonical_name().value_or(string_null)
       << ')';
    return os;
}
