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

#include "network/connect-endpoint.h"   // connect()
#include "network/endpoint.h"           // Endpoint
#include "network/open-endpoint.h"      // open()
#include "network/openendpointparams.h" // OpenEndpointParams
#include "network/openhandler.h"        // OpenHandler
#include "network/openresult.h"         // OpenResult
#include "network/sockethints.h"        // SocketHints

#ifdef WIN32
#include <winsock2.h>       // ::connect()
#else
#include <sys/socket.h>     // ::connect()
#endif

auto Network::connect(const Endpoint& endpoint,
                      const SocketHints& hints,
                      bool verbose) -> OpenResult
{
    const OpenHandler handler {::connect, "::connect"};
    const OpenEndpointParams args {endpoint, hints, verbose};
    return open(handler, args);
}
