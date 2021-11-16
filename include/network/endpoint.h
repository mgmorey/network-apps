#ifndef NETWORK_ENDPOINT_H
#define NETWORK_ENDPOINT_H

#include "network/result.h"         // Result
#include "network/types.h"          // Hostname, Service, SockAddr

#include <utility>      // std::pair
#include <variant>      // std::variant

namespace Network
{
    using Endpoint = std::pair<Hostname, Service>;

    using EndpointResult = std::variant<Endpoint, Result>;

    EndpointResult get_endpoint(const SockAddr& addr, int flags,
                                bool verbose = false);
}

#endif
