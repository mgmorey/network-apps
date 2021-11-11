#ifndef NETWORK_HOSTNAME_H
#define NETWORK_HOSTNAME_H

#include "network-endpoint.h"   // Endpoint, EndpointResult
#include "network-result.h"     // Result
#include "network-types.h"      // Hostname

#include <variant>      // std::variant

namespace Network
{
    using HostnameResult = std::variant<Hostname, Result>;

    extern HostnameResult get_hostname();
    extern HostnameResult get_hostname(const Hostname& hostname);
    extern HostnameResult get_hostname(const Endpoint& endpoint);
}

#endif
