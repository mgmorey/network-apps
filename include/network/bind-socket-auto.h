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

#ifndef NETWORK_BIND_SOCKET_AUTO_H
#define NETWORK_BIND_SOCKET_AUTO_H

#include "network/bind-socket-bs.h"             // bind()
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/socket.h"                     // Socket
#include "network/to-bytestring-auto.h"         // to_bytestring()

namespace Network
{
    auto bind(const Socket &sock,
              const auto &value,
              bool verbose = false) -> OsErrorResult
    {
        return Network::bind(sock, to_bytestring(value), verbose);
    }
}

#endif
