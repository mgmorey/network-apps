#include "network-sockets.h"    // Address, Endpoint, Hints,
                                // SocketsResult
#include "network-addrinfo.h"   // AddrInfo

Network::SocketsResult Network::get_sockets(const Network::Endpoint& endpoint,
                                            const Network::Hints* hints,
                                            bool verbose)
{
    return AddrInfo::get<SocketsResult>(endpoint, hints, verbose);
}
