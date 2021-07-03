#ifndef NETWORK_ENDPOINT_H
#define NETWORK_ENDPOINT_H

#include "network-result.h"     // Result
#include "network-types.h"      // Hostname, Service

#include <utility>      // std::pair

namespace Network
{
    typedef std::pair<Hostname, Service> Endpoint;
    typedef std::pair<Endpoint, Result> EndpointResult;
}

#endif
