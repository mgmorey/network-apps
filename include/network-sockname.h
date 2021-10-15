#ifndef NETWORK_SOCKNAME_H
#define NETWORK_SOCKNAME_H

#include "network-fd.h"         // Fd
#include "network-result.h"     // Result
#include "network-types.h"      // SockAddr

#include <utility>      // std::pair

namespace Network
{
    extern std::pair<SockAddr, Result> get_sockname(Fd fd, bool verbose = false);
}

#endif
