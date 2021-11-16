#include "network/sockets.h"        // Sockets, SocketResults,
                                    // SocketsResult, get_sockets()
#include "network/addrinfo.h"       // AddrInfo
#include "network/open.h"           // Endpoint, Fd, Hints, Result,
                                    // Socket, SockAddr, SocketResult,
                                    // fd_null, get_sockets(), open(),
                                    // operator<<()
#include "network/overload.h"       // Overload

#include <variant>      // std::visit()

Network::SocketsResult
Network::get_sockets(const Network::Endpoint& endpoint,
                     const Network::Hints* hints,
                     bool verbose)
{
    SocketsResult sockets_result;
    const auto hostname_result {get_hostname(endpoint.first)};
    std::visit(Network::Overload {
            [&](const std::string& host) {
                sockets_result = get_sockets(host,
                                             endpoint.second,
                                             hints,
                                             verbose);
            },
            [&](const Network::Result& result) {
                sockets_result = result;
            }
        }, hostname_result);
    return sockets_result;
}

Network::SocketsResult
Network::get_sockets(const Network::Hostname& node,
                     const Network::Service& serv,
                     const Network::Hints* hints,
                     bool verbose)
{
    Sockets sockets;
    const auto result {AddrInfo::insert(node, serv, hints, verbose,
                                        std::back_inserter(sockets))};

    if (result) {
        return result;
    }

    return sockets;
}
