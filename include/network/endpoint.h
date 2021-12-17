#ifndef NETWORK_ENDPOINT_H
#define NETWORK_ENDPOINT_H

#include "network/hostname.h"           // Hostname
#include "network/service.h"            // Service

#include <utility>      // std::pair

namespace Network
{
    using Endpoint = std::pair<Hostname, Service>;
}

#endif
