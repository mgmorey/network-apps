#include "network-sockets.h"    // Endpoint, Hostname, Service,
                                // SocketsResult, struct addrinfo
#include "network-addrinfo.h"   // insert_addrinfo()
#include "network-hostname.h"   // get_hostname()

#include <iterator>     // std::back_inserter()

static Network::Hostname get_node(const Network::Endpoint& endpoint)
{
    if (endpoint.first.empty() && endpoint.second.empty()) {
        return Network::get_hostname().first;
    }

    return endpoint.first;
}

Network::SocketsResult Network::get_sockets(const Endpoint& endpoint,
                                            const addrinfo* hints,
                                            bool verbose)
{
    SocketsResult result;
    const Hostname node(get_node(endpoint));
    const Service service(endpoint.second);
    result.second = insert_addrinfo(std::back_inserter(result.first),
                                    node, service, hints, verbose);
    return result;
}

Network::SocketsResult Network::get_sockets(const Endpoint& endpoint,
                                            const addrinfo& hints,
                                            bool verbose)
{
    return get_sockets(endpoint, &hints, verbose);
}
