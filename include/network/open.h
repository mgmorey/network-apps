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

#ifndef NETWORK_OPEN_H
#define NETWORK_OPEN_H

#include "network/bytes.h"              // Bytes
#include "network/endpoint.h"           // Endpoint
#include "network/fdresultvector.h"     // Fd, FdResultVector, Result
#include "network/hints.h"              // Hints
#include "network/openhandler.h"        // OpenHandler, sockaddr,
                                        // socklen_t

namespace Network
{
    extern auto open(const OpenHandler& handler,
                     Fd fd,
                     const Bytes& addr,
                     bool verbose) -> Result;
    extern auto open(const OpenHandler& handler,
                     const Endpoint& endpoint,
                     const Hints& hints,
                     bool verbose) -> FdResultVector;
}

#endif
