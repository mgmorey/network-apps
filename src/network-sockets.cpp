#include "network-sockets.h"    // Endpoint, Hostname, Service,
                                // SocketsResult, struct addrinfo
#include "network-addrinfo.h"   // AddrInfo, operator<<()
#include "network-hostname.h"   // get_hostname()

Network::SocketsResult Network::get_sockets(const Endpoint& endpoint,
                                            const Hints* hints,
                                            bool verbose)
{
    return AddrInfo::get<SocketsResult>(endpoint, hints, verbose);
}
