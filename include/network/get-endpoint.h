#ifndef NETWORK_GET_ENDPOINT_H
#define NETWORK_GET_ENDPOINT_H

#include "network/bytes.h"              // Bytes
#include "network/endpointresult.h"     // EndpointResult

namespace Network
{
    extern auto get_endpoint(const Bytes& addr, int flags,
                             bool verbose = false) -> EndpointResult;
}

#endif
