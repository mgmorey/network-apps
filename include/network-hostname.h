#ifndef NETWORK_HOSTNAME_H
#define NETWORK_HOSTNAME_H

#include "network-endpoint.h"   // Endpoint, EndpointResult
#include "network-result.h"     // Result
#include "network-types.h"      // Hostname

#include <utility>      // std::pair

namespace Network
{
    typedef std::pair<Hostname, Result> HostnameResult;
    extern HostnameResult get_hostname();
    extern Hostname get_hostname(const Hostname& hostname);
    extern Hostname get_hostname(const Endpoint& endpoint);
}

#endif
