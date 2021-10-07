#ifndef NETWORK_PEERNAME_H
#define NETWORK_PEERNAME_H

#include "network-address.h"    // Address, sockaddr, sock_len_type
#include "network-buffer.h"     // Buffer
#include "network-fd.h"         // Fd
#include "network-result.h"     // Result

#include <utility>      // std::pair

namespace Network
{
    using AddressResult = std::pair<Address, Result>;

    extern AddressResult get_peername(Fd fd, bool verbose = false);
}

#endif
