#ifndef NETWORK_ENDPOINT_H
#define NETWORK_ENDPOINT_H

#include "network-types.h"      // Hostname, Service

#include <utility>      // std::pair

namespace Network
{
    typedef std::pair<Hostname, Service> Endpoint;

    extern Hostname get_hostname(const Endpoint& endpoint);
    extern Service get_service(const Endpoint& endpoint);
}

#endif
