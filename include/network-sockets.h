#ifndef NETWORK_SOCKETS_H
#define NETWORK_SOCKETS_H

#include "network-endpoint.h"   // Endpoint
#include "network-socket.h"     // Fd, Hints, Result, Socket,
                                // SocketResult

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
