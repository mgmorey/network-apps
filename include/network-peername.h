#ifndef NETWORK_PEERNAME_H
#define NETWORK_PEERNAME_H

#include "network-address.h"    // Address
#include "network-fd.h"         // sock_fd_type
#include "network-result.h"     // Result

#include <utility>      // std::pair

namespace Network
{
    typedef std::pair<Address, Result> AddressResult;
    extern AddressResult get_peername(sock_fd_type fd, bool verbose = false);
}

#endif
