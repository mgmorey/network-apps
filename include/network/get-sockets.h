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

#ifndef NETWORK_GET_SOCKETS_H
#define NETWORK_GET_SOCKETS_H

#include "network/endpoint.h"           // Endpoint
#include "network/hints.h"              // Hints
#include "network/optionalhostname.h"   // OptionalHostname
#include "network/optionalservice.h"    // OptionalService
#include "network/result.h"             // Result
#include "network/socketvector.h"       // SocketVector

#include <variant>      // std::variant

namespace Network
{
    using SocketVectorResult = std::variant<SocketVector, Result>;

    extern auto get_sockets(const Endpoint& endpoint,
                            const Hints& hints,
                            bool verbose) -> SocketVectorResult;
    extern auto get_sockets(const OptionalHostname& hostname,
                            const OptionalService& service,
                            const Hints& hints,
                            bool verbose) -> SocketVectorResult;
}

#endif
