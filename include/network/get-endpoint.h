#ifndef NETWORK_ENDPOINTRESULT_H
#define NETWORK_ENDPOINTRESULT_H

#include "network/endpointresult.h" // Endpoint
#include "network/types.h"          // SockAddr

namespace Network
{
    EndpointResult get_endpoint(const SockAddr& addr, int flags,
                                bool verbose = false);
}

#endif
