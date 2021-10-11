#ifndef NETWORK_PEERNAME_H
#define NETWORK_PEERNAME_H

#include "network-address.h"    // Address
#include "network-fd.h"         // Fd
#include "network-result.h"     // Result

#include <utility>      // std::pair

namespace Network
{
    extern std::pair<Address, Result> get_peername(Fd fd, bool verbose = false);
}

#endif
