#include "network/open.h"       // Endpoint, Fd, Hints, Result,
                                // Socket, SockAddr, SocketResult,
                                // fd_null, get_sockets(), open(),
                                // operator<<()
#include "network/addrinfo.h"   // AddrInfo

#include <variant>      // std::get(), std::holds_alternative()

Network::SocketsResult
Network::get_sockets(const Network::Endpoint& endpoint,
                     const Network::Hints* hints,
                     bool verbose)
{
    SocketsResult sockets_result;
    const auto hostname_result {Network::get_hostname(endpoint.first)};

    if (std::holds_alternative<Result>(hostname_result)) {
        return std::get<Result>(hostname_result);
    }
    else if (std::holds_alternative<Hostname>(hostname_result)) {
        const auto host {std::get<Network::Hostname>(hostname_result)};
        sockets_result = get_sockets(host, endpoint.second, hints, verbose);
    }
    else {
        abort();
    }

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
