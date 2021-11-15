#include "network/open.h"       // Endpoint, Fd, Hints, OpenHandler,
                                // Result, SockAddr, SocketResult,
                                // SocketResults, Sockets,
                                // get_sockets(), open(), operator<<()
#include "network/overload.h"   // Overload

#include <algorithm>    // std::transform()
#include <iterator>     // std::back_inserter()
#include <variant>      // std::visit()

Network::SocketResults
Network::open(const OpenHandler& handler,
              const Endpoint& endpoint,
              const Hints* hints,
              bool verbose)
{
    const auto lambda = [&](const Socket& sock) {
        auto socket_result {get_socket(sock, verbose)};
        std::visit(Overload {
                [&](Fd fd) {
                    const auto addr {sock.address()};
                    const auto result = open(handler, fd, addr, verbose);

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
    SocketResults results;
    std::visit(Overload {
            [&](const Sockets& sockets) {
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
