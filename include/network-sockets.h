#ifndef NETWORK_SOCKETS_H
#define NETWORK_SOCKETS_H

#include "network-endpoint.h"   // Address, Endpoint
#include "network-hints.h"      // Hints
#include "network-result.h"     // Result
#include "network-socket.h"     // Socket, SocketResult

#include <vector>       // std::vector

namespace Network
{
    using Sockets = std::vector<Socket>;
    using SocketsResult = std::pair<Sockets, Result>;

    extern SocketsResult get_sockets(const Endpoint& endpoint,
                                     const Hints* hints,
                                     bool verbose);
}

#endif
