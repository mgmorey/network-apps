#ifndef NETWORK_ENDPOINT_H
#define NETWORK_ENDPOINT_H

#include "network-result.h"     // Result
#include "network-types.h"      // Hostname, Service, SockAddr

#include <utility>      // std::pair

namespace Network
{
    using Endpoint = std::pair<Hostname, Service>;
    using EndpointResult = std::pair<Endpoint, Result>;

    EndpointResult get_endpoint(const SockAddr& sock_addr, int flags,
                                bool verbose = false);
    EndpointResult get_endpoint(const SockAddr& sock_addr, bool numeric = false,
                                bool verbose = false);
}

#endif
