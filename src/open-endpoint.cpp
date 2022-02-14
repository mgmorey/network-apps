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

#include "network/open-endpoint.h"      // Endpoint, FdResultVector,
                                        // Hints, OpenEndpointParams,
                                        // OpenHandler, OpenResult,
                                        // open()
#include "network/open-fd.h"            // Bytes, Fd, OpenHandler,
                                        // OsErrorResult, open()
#include "network/get-socket.h"         // ErrorResult, get_socket()
#include "network/get-sockets.h"        // SocketVector, get_sockets()
#include "network/overloaded.h"         // Overloaded

#include <algorithm>    // std::transform()
#include <iterator>     // std::back_inserter()
#include <variant>      // std::visit()

auto Network::open(const OpenHandler& handler,
                   const OpenEndpointParams& args) -> Network::OpenResult
{
    OpenResult open_result;
    FdResultVector fd_results;
    const auto lambda = [&](const Socket& sock) {
        auto socket_result {get_socket(sock, args.verbose)};
        std::visit(Overloaded {
                [&](Fd fd) {
                    const auto addr {sock.address()};
                    const auto params {OpenFdParams {fd, addr, args.verbose}};
                    const auto result {open(handler, params)};

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
    const auto sockets_result {get_sockets(args.endpoint,
                                           args.hints,
                                           args.verbose)};
    std::visit(Overloaded {
            [&](const SocketVector& sockets) {
                std::transform(sockets.begin(), sockets.end(),
                               std::back_inserter(fd_results),
                               lambda);
                open_result = OpenResult {fd_results};
            },
            [&](const OsErrorResult& result) {
                open_result = OpenResult {result};
            }
        }, sockets_result);
    return open_result;
}
