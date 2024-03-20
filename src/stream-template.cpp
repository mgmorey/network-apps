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

#include "network/template.h"           // Template, operator<<()
#include "network/format.h"             // Format, operator<<()
#include "network/string-null.h"        // string_null
#include "network/to-hex.h"             // to_hex()

#include <ostream>      // operator<<(), std::ostream

auto Network::operator<<(std::ostream& os,
                         const Template& sock) noexcept -> std::ostream&
{
    static constexpr auto tab {7};

    os << "Template("
       << Format("flags")
       << sock.flags()
       << Format(tab, "family")
       << sock.family()
       << Format(tab, "socktype")
       << sock.socktype()
       << Format(tab, "protocol")
       << sock.protocol()
       << Format(tab, "address")
       << to_hex(sock.address())
       << Format(tab, "canonical_name")
       << sock.canonical_name().value_or(string_null)
       << ')';
    return os;
}
