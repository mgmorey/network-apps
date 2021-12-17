#ifndef NETWORK_GET_SOCKETS_H
#define NETWORK_GET_SOCKETS_H

#include "network/endpoint.h"           // Endpoint
#include "network/hints.h"              // Hints
#include "network/hostname.h"           // Hostname
#include "network/result.h"             // Result
#include "network/service.h"            // Service
#include "network/socketvector.h"       // SocketVector

#include <variant>      // std::variant

namespace Network
{
    using SocketVectorResult = std::variant<SocketVector, Result>;

    extern auto get_sockets(const Endpoint& endpoint,
                            const Hints* hints,
                            bool verbose) -> SocketVectorResult;
    extern auto get_sockets(const Hostname& node,
                            const Service& serv,
                            const Hints* hints,
                            bool verbose) -> SocketVectorResult;
}

#endif
