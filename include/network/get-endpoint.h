#ifndef NETWORK_GET_ENDPOINT_H
#define NETWORK_GET_ENDPOINT_H

#include "network/bytes.h"              // Bytes
#include "network/endpointresult.h"     // EndpointResult
#include "network/sockaddr.h"           // SockAddr

namespace Network
{
    extern auto get_endpoint(const SockAddr& addr, int flags,
                             bool verbose = false) -> EndpointResult;
}

#endif
