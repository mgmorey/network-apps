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

#include "network/get-sockets-hostname.h"       // OptionalHints,
                                                // OptionalHostname,
                                                // OptionalService,
                                                // SocketVector,
                                                // SocketVectorResult,
                                                // get_sockets()

#include "network/addrinfo.h"           // AddrInfo

#include <iterator>     // std::back_inserter()

auto Network::get_sockets(const OptionalHostname& hostname,
                          const OptionalService& service,
                          const OptionalHints& hints,
                          bool verbose) -> Network::SocketVectorResult
{
    SocketVector sockets;
    const auto result {
        AddrInfo::insert(hostname,
                         service,
                         hints,
                         std::back_inserter(sockets),
                         verbose)
    };

    if (result) {
        return result;
    }

    return sockets;
}
