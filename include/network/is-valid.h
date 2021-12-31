#ifndef NETWORK_IS_VALID_H
#define NETWORK_IS_VALID_H

#include "network/bytes.h"              // Bytes

namespace Network
{
    extern auto is_valid(const Bytes& addr,
                         bool verbose = false) -> bool;
}

#endif
