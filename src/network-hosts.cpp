#include "network-hosts.h"      // HostsResult, Hostname
#include "network-addrinfo.h"   // insert_addrinfo()
#include "network-hostname.h"   // get_hostname()

#include <iterator>     // std::back_inserter()

static Network::Hostname get_node(const Network::Hostname& hostname)
{
    if (hostname.empty()) {
        return Network::get_hostname().first;
    }

    return hostname;
}

Network::HostsResult Network::get_hosts(const Hostname& host,
                                        const addrinfo* hints,
                                        bool verbose)
{
    HostsResult result;
    const auto node(get_node(host));
    auto it(std::back_inserter(result.first));
    result.second = AddrInfo::insert(node, "", hints, verbose, it);
    return result;
}

Network::HostsResult Network::get_hosts(const Hostname& host,
                                        const addrinfo& hints,
                                        bool verbose)
{
    return get_hosts(host, &hints, verbose);
}
