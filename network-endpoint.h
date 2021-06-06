#ifndef NETWORK_ENDPOINT_H
#define NETWORK_ENDPOINT_H

#include "network-types.h"

#include <string>       // std::string
#include <utility>      // std::pair

namespace Network
{
    typedef std::pair<Hostname, Service> Endpoint;

    extern Hostname hostname(const Endpoint& endpoint);
    extern Service service(const Endpoint& endpoint);
}

#endif
