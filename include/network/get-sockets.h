#ifndef NETWORK_GET_SOCKETS_H
#define NETWORK_GET_SOCKETS_H

#include "network/endpoint.h"   // Endpoint
#include "network/hints.h"      // Hints
#include "network/socket.h"     // Socket, SocketResult

#include <utility>      // std::pair
#include <variant>      // std::variant
#include <vector>       // std::vector

namespace Network
{
    using Sockets = std::vector<Socket>;
    using SocketResults = std::vector<SocketResult>;
    using SocketsResult = std::variant<Sockets, Result>;

    extern SocketsResult get_sockets(const Endpoint& endpoint,
                                     const Hints* hints,
                                     bool verbose);
    extern SocketsResult get_sockets(const Hostname& node,
                                     const Service& serv,
                                     const Hints* hints,
                                     bool verbose);
}

#endif
