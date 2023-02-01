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

#include "network/bind-socket.h"        // ByteString, OsErrorResult,
                                        // Socket, bind(), open()
#include "network/openhandler.h"        // OpenHandler
#include "network/opensocketparams.h"   // OpenSocketParams

#ifdef WIN32
#include <winsock2.h>       // ::bind()
#else
#include <sys/socket.h>     // ::bind()
#endif

auto Network::bind(const Socket& socket,
                   const ByteString& str,
                   bool verbose) -> Network::OsErrorResult
{
    const OpenHandler handler {::bind, "::bind"};
    const OpenSocketParams args {socket, str, verbose};
    return open(handler, args);
}
