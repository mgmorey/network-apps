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

#include "network/socket.hpp"                   // Socket, operator<<()
#include "network/long-handle-type.hpp"         // long_handle_type
#include "network/string-null.hpp"              // string_null

#include <ostream>      // std::ostream

auto Network::operator<<(std::ostream& os,
                         const Socket& s) -> std::ostream&
{
    if (static_cast<bool>(s)) {
        os << static_cast<long_handle_type>(s);
    }
    else {
        os << string_null;
    }

    return os;
}
