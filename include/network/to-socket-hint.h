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

#ifndef NETWORK_TO_SOCKET_HINT_H
#define NETWORK_TO_SOCKET_HINT_H

#include "network/socket-hint-type.h"           // socket_hint_type
#include "network/sockethinterror.h"            // SocketHintError

#include <string>       // std::to_string()
#include <utility>      // std::in_range()

namespace Network
{
    auto to_socket_hint(auto value) -> socket_hint_type
    {
        if (!std::in_range<socket_hint_type>(value)) {
            throw SocketHintError(std::to_string(value));
        }

        return static_cast<socket_hint_type>(value);
    }
}

#endif
