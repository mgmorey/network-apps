#include "network-sockets.h"    // Hostname, Service, Sockets,
                                // SocketsResult, get_sockets()
#include "network-addrinfo.h"   // get_addrinfo()
#include "network-hostname.h"   // get_hostname()

#include <string>       // std::string

Network::SocketsResult Network::get_sockets(const Hostname& host,
                                            const Service& service,
                                            const struct addrinfo& hints)
{
    std::string error;
    Hostname node(host);

    if (host.empty() && service.empty()) {
        node = get_hostname().first;
    }

    Sockets sockets(get_addrinfo<Sockets>(error, node, service, &hints));
    return SocketsResult(sockets, error);
}
