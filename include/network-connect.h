#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network-endpoint.h"   // Endpoint
#include "network-fd.h"         // Fd
#include "network-hints.h"      // Hints
#include "network-result.h"     // Result
#include "network-socket.h"     // Socket, SocketResult
#include "network-types.h"      // SockAddr, operator<<()

#include <vector>       // std::vector

namespace Network
{
    extern Result connect(const Fd& fd,
                          const SockAddr& sock_addr,
                          bool verbose = false);
    extern std::vector<SocketResult> connect(const Endpoint& endpoint,
                                             const Hints* hints = nullptr,
                                             bool verbose = false);
}

#endif
