#ifndef NETWORK_ENDPOINT_H
#define NETWORK_ENDPOINT_H

#include "network-address.h"    // Address
#include "network-result.h"     // Result
#include "network-types.h"      // Hostname, Service

#include <utility>      // std::pair

namespace Network
{
    typedef std::pair<Hostname, Service> Endpoint;
    typedef std::pair<Endpoint, Result> EndpointResult;

    extern EndpointResult to_endpoint(const Address& address,
                                      int flags, bool verbose = false);
    extern EndpointResult to_endpoint(const Address& address,
                                      bool numeric = false,
                                      bool verbose = false);
}

#endif
