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

#ifndef WIN32

#include "network/create-socket-handle.h"       // create_socket()
#include "network/handle-type.h"                // handle_type
#include "network/socket-family-type.h"         // socket_family_type
#include "network/socketdata.h"                 // SocketData
#include "network/uniquesocket.h"               // UniqueSocket
#include "network/unixsocket.h"                 // UnixSocket

#include <sys/socket.h>     // AF_UNIX

#include <memory>       // std::make_shared()

auto Network::create_socket(const SocketData& data) -> UniqueSocket
{
    switch (data.m_family) {  // NOLINT
    case AF_UNIX:
        return std::make_unique<UnixSocket>(data);
    default:
        return std::make_unique<CommonSocket>(data);
    }
}

auto Network::create_socket(socket_family_type family,
                            handle_type handle,
                            bool is_verbose) -> UniqueSocket
{
    switch (family) {  // NOLINT
    case AF_UNIX:
        return std::make_unique<UnixSocket>(family, handle, is_verbose);
    default:
        return std::make_unique<CommonSocket>(family, handle, is_verbose);
    }
}

#endif
