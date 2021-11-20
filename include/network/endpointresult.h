#ifndef NETWORK_ENDPOINTRESULT_H
#define NETWORK_ENDPOINTRESULT_H

#include "network/endpoint.h"       // Endpoint, Result

#include <variant>      // std::variant

namespace Network
{
    using EndpointResult = std::variant<Endpoint, Result>;
}

#endif
