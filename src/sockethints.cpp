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

#include "network/format.h"             // Format, operator<<()
#include "network/sockethints.h"        // SocketHints, operator<<(),
                                        // std::ostream

auto Network::operator<<(std::ostream& os,
                         const SocketHints& hints ) noexcept -> std::ostream&
{
    static constexpr auto delim {", "};
    static constexpr auto tab {12};

    os << "SocketHints("
       << Format("flags")
       << hints.flags()
       << Format(delim, tab, "family")
       << hints.family()
       << Format(delim, tab, "socktype")
       << hints.socktype()
       << Format(delim, tab, "protocol")
       << hints.protocol()
       << ')';
    return os;
}
