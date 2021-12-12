#include "network/open.h"           // Endpoint, Fd, FdResult,
                                    // FdResultVector, Hints,
                                    // OpenHandler, Result,
                                    // get_socket(), open(),
                                    // operator<<()
#include "network/get-socket.h"     // FdResult, get_socket()
#include "network/get-sockets.h"    // SocketVector,
                                    // SocketVectorResult,
                                    // get_sockets()
#include "network/overload.h"       // Overload

#include <algorithm>    // std::transform()
#include <iterator>     // std::back_inserter()
#include <variant>      // std::visit()

auto Network::open(const OpenHandler& handler,
                   const Endpoint& endpoint,
                   const Hints* hints,
                   bool verbose) -> Network::FdResultVector
{
    FdResultVector results;
    const auto lambda = [&](const Socket& sock) {
        auto socket_result {get_socket(sock, verbose)};
        std::visit(Overload {
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
                [&](const Result&) {
                }
            }, socket_result);
        return socket_result;
    };
    const auto sockets_result {get_sockets(endpoint, hints, verbose)};
    std::visit(Overload {
            [&](const SocketVector& sockets) {
                std::transform(sockets.begin(), sockets.end(),
                               std::back_inserter(results),
                               lambda);
            },
            [&](const Result& result) {
                results.push_back(result);
            }
        }, sockets_result);
    return results;
}
