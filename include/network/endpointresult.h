#ifndef NETWORK_GET_ENDPOINT_H
#define NETWORK_GET_ENDPOINT_H

#include "network/endpoint.h"       // Endpoint, Result

#include <variant>      // std::variant

namespace Network
{
    using EndpointResult = std::variant<Endpoint, Result>;
}

#endif
