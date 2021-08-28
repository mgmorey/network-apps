#include "network-sockets.h"    // Endpoint, Hostname, Service,
                                // SocketsResult, struct addrinfo
#include "network-addrinfo.h"   // insert_addrinfo()
#include "network-hostname.h"   // get_hostname()

#include <iterator>     // std::back_inserter(),
                        // std::back_insert_iterator()

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
    std::back_insert_iterator<Sockets> it(std::back_inserter(result.first));
    result.second = insert_addrinfo(node, service, hints, verbose, it);
    return result;
}

Network::SocketsResult Network::get_sockets(const Endpoint& endpoint,
                                            const addrinfo& hints,
                                            bool verbose)
{
    return get_sockets(endpoint, &hints, verbose);
}
