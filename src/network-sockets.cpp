#include "network-sockets.h"    // Endpoint, Hostname, Service,
                                // SocketsResult, struct addrinfo,
                                // get_sockets()
#include "network-addrinfo.h"   // get_addrinfo()
#include "network-hostname.h"   // get_hostname()

static Network::Hostname get_node(const Network::Endpoint& endpoint)
{
    if (endpoint.first.empty() && endpoint.second.empty()) {
        return Network::get_hostname().first;
    }

    return endpoint.first;
}

Network::SocketsResult Network::get_sockets(const Endpoint& endpoint,
                                            const addrinfo& hints,
                                            bool verbose)
{
    const Hostname node(get_node(endpoint));
    const Service service(endpoint.second);
    return get_addrinfo<SocketsResult>(node, service, &hints, false, verbose);
}

Network::SocketsResult Network::get_sockets(const Endpoint& endpoint,
                                            bool verbose)
{
    const Hostname node(get_node(endpoint));
    const Service service(endpoint.second);
    return get_addrinfo<SocketsResult>(node, service, NULL, false, verbose);
}
