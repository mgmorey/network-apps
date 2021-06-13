#include "network-sockets.h"    // Hostname, Service, SocketsResult,
                                // struct addrinfo, get_sockets()
#include "network-addrinfo.h"   // get_addrinfo()
#include "network-hostname.h"   // get_hostname()

Network::SocketsResult Network::get_sockets(const Hostname& host,
                                            const Service& service,
                                            const struct addrinfo& hints)
{
    Hostname node(host);

    if (host.empty() && service.empty()) {
        node = get_hostname().first;
    }

    return get_addrinfo<SocketsResult>(node, service, &hints);
}
