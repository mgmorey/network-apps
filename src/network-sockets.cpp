#include "network-sockets.h"    // Address, Endpoint, Hints,
                                // SocketsResult
#include "network-addrinfo.h"   // AddrInfo

Network::SocketsResult get_sockets(const Network::Endpoint& endpoint,
                                   const Network::Hints* hints,
                                   bool verbose)
{
    return Network::AddrInfo::get<Network::SocketsResult>
        (endpoint, hints, verbose);
}
