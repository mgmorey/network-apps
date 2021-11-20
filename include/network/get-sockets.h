#ifndef NETWORK_GET_SOCKETS_H
#define NETWORK_GET_SOCKETS_H

#include "network/endpoint.h"       // Endpoint
#include "network/hints.h"          // Hints
#include "network/result.h"         // Result
#include "network/socketvector.h"   // SocketVector
#include "network/types.h"          // Hostname, Service

#include <variant>      // std::variant

namespace Network
{
    using SocketVectorResult = std::variant<SocketVector, Result>;

    extern SocketVectorResult get_sockets(const Endpoint& endpoint,
                                          const Hints* hints,
                                          bool verbose);
    extern SocketVectorResult get_sockets(const Hostname& node,
                                          const Service& serv,
                                          const Hints* hints,
                                          bool verbose);
}

#endif
