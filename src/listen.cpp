// Copyright (C) 2023  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/listen.h"                     // listen()
#include "network/context-error-type.h"         // context_error_type
#include "network/socket-type.h"                // socket_type
#include "network/socket.h"                     // Socket

#ifdef WIN32
#include <winsock2.h>       // ::listen()
#else
#include <sys/socket.h>     // ::listen()
#endif

auto Network::listen(const Socket& sock, int backlog_size) -> context_error_type
{
    return ::listen(socket_type {sock}, backlog_size);
}
