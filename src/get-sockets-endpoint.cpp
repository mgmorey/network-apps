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

#include "network/get-sockets-endpoint.h"       // Endpoint,
                                                // OptionalHints,
                                                // SocketVectorResult,
                                                // get_sockets()
#include "network/get-sockets-hostname.h"       // OptionalHostname,
                                                // OptionalService,
                                                // get_sockets()

auto Network::get_sockets(const Endpoint& endpoint,
                          const OptionalHints& hints,
                          bool verbose) -> Network::SocketVectorResult
{
    const auto& hostname {endpoint.first};
    const auto& service {endpoint.second};
    return get_sockets(hostname, service, hints, verbose);
}
