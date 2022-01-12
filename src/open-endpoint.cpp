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

#include "network/open.h"               // Endpoint, ErrorResult, Fd,
                                        // FdResultVector, Hints,
                                        // OpenHandler, OsErrorResult,
                                        // open(), operator<<()
#include "network/get-socket.h"         // ErrorResult, get_socket()
#include "network/get-sockets.h"        // SocketVector, get_sockets()
#include "network/overloaded.h"         // Overloaded

#include <algorithm>    // std::transform()
#include <iterator>     // std::back_inserter()
#include <variant>      // std::visit()

auto Network::open(const OpenHandler& handler,
                   const Endpoint& endpoint,
                   const Hints& hints,
                   bool verbose) -> Network::FdResultVector
{
    FdResultVector results;
    const auto lambda = [&](const Socket& sock) {
        auto socket_result {get_socket(sock, verbose)};
        std::visit(Overloaded {
                [&](Fd fd) {
                    const auto addr {sock.address()};
                    const auto result {open(handler, fd, addr, verbose)};

                    if (result) {
                        socket_result = result;
                    }
                    else {
                        socket_result = fd;
                    }
                },
                [&](const OsErrorResult& result) {
                    static_cast<void>(result);
                }
            }, socket_result);
        return socket_result;
    };
    const auto sockets_result {get_sockets(endpoint, hints, verbose)};
    std::visit(Overloaded {
            [&](const SocketVector& sockets) {
                std::transform(sockets.begin(), sockets.end(),
                               std::back_inserter(results),
                               lambda);
            },
            [&](const ErrorResult& result) {
                results.push_back(result);
            }
        }, sockets_result);
    return results;
}
