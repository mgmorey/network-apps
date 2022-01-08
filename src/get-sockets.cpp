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

#include "network/get-sockets.h"    // Hints, Hostname, Result,
                                    // Service, SocketVectorResult,
                                    // get_sockets()
#include "network/addrinfo.h"       // AddrInfo
#include "network/get-hostname.h"   // HostnameResult, get_hostname()
#include "network/overloaded.h"     // Overloaded

#include <iterator>     // std::back_inserter()
#include <string>       // std::string
#include <variant>      // std::visit()

auto Network::get_sockets(const Network::Endpoint& endpoint,
                          const Network::Hints* hints,
                          bool verbose) -> Network::SocketVectorResult
{
    SocketVectorResult sockets_result;
    const auto hostname_result {get_hostname(endpoint.first)};
    std::visit(Overloaded {
            [&](const std::string& host) {
                sockets_result = get_sockets(host,
                                             endpoint.second,
                                             hints,
                                             verbose);
            },
            [&](const Result& result) {
                sockets_result = result;
            }
        }, hostname_result);
    return sockets_result;
}

auto Network::get_sockets(const Network::Hostname& node,
                          const Network::Service& serv,
                          const Network::Hints* hints,
                          bool verbose) -> Network::SocketVectorResult
{
    SocketVector sockets;
    const auto result {
        AddrInfo::insert(node,
                         serv,
                         hints,
                         verbose,
                         std::back_inserter(sockets))
    };

    if (result) {
        return result;
    }

    return sockets;
}
