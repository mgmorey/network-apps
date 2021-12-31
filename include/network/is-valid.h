#ifndef NETWORK_IS_VALID_H
#define NETWORK_IS_VALID_H

#include "network/bytes.h"              // Bytes
#include "network/sockaddr.h"           // SockAddr

namespace Network
{
    extern auto is_valid(const SockAddr& addr,
                         bool verbose = false) -> bool;
}

#endif
