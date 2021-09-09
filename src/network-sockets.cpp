#include "network-sockets.h"    // Endpoint, Hostname, Service,
                                // SocketsResult, struct addrinfo
#include "network-addrinfo.h"   // AddrInfo, operator<<()
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
                                            const Hints* hints,
                                            bool verbose)
{
    SocketsResult result;
    const auto node(get_node(endpoint));
    const auto service(endpoint.second);
    auto it(std::back_inserter(result.first));
    result.second = AddrInfo::insert(node, service, hints, verbose, it);
    return result;
}
