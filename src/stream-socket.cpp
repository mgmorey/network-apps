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

#include "network/sharedsocket.h"       // SharedSocket, operator<<()
#include "network/handle-null.h"        // handle_null
#include "network/string-null.h"        // string_null

#include <ostream>      // std::ostream

auto Network::operator<<(std::ostream& os,
                         const SharedSocket& sock) -> std::ostream&
{
    const auto handle {sock->handle()};

    if (handle == handle_null) {
        return os << string_null;
    }

    return os << handle;
}
