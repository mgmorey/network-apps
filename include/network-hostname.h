#ifndef NETWORK_HOSTNAME_H
#define NETWORK_HOSTNAME_H

#include "network-endpoint.h"   // Endpoint, EndpointResult
#include "network-result.h"     // Result
#include "network-types.h"      // Hostname

#include <utility>      // std::pair

namespace Network
{
    using HostnameResult = std::pair<Hostname, Result>;

    extern HostnameResult get_hostname();
    extern Hostname get_hostname(const Hostname& hostname);
    extern Hostname get_hostname(const Endpoint& endpoint);
}

#endif
