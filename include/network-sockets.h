#ifndef NETWORK_SOCKETS_H
#define NETWORK_SOCKETS_H

#include "network-endpoint.h"   // Endpoint
#include "network-hints.h"      // Hints
#include "network-result.h"     // Result
#include "network-socket.h"     // Socket
#include "network-types.h"      // Hostname, Service

#include <utility>      // std::pair
#include <vector>       // std::vector

namespace Network
{
    typedef std::vector<Socket> Sockets;
    typedef std::pair<Sockets, Result> SocketsResult;
    extern SocketsResult get_sockets(const Endpoint& endpoint,
                                     const Hints* hints = nullptr,
                                     bool verbose = false);
    extern SocketsResult get_sockets(const Endpoint& endpoint,
                                     const Hints& hints,
                                     bool verbose = false);
}

#endif
