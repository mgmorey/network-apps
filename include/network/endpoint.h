#ifndef NETWORK_ENDPOINT_H
#define NETWORK_ENDPOINT_H

#include "network/types.h"          // Hostname, Service

#include <utility>      // std::pair

namespace Network
{
    using Endpoint = std::pair<Hostname, Service>;
}

#endif
