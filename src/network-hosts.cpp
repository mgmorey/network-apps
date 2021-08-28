#include "network-hosts.h"      // HostsResult, Hostname
#include "network-addrinfo.h"   // insert_addrinfo()
#include "network-hostname.h"   // get_hostname()

#include <iterator>     // std::back_inserter(),
                        // std::back_insert_iterator()

Network::HostsResult Network::get_hosts(const Hostname& host,
                                        const addrinfo* hints,
                                        bool verbose)
{
    HostsResult result;
    const Hostname node(host.empty() ? get_hostname().first : host);
    std::back_insert_iterator<Hosts> it(std::back_inserter(result.first));
    result.second = insert_addrinfo(node, "", hints, verbose, it);
    return result;
}

Network::HostsResult Network::get_hosts(const Hostname& host,
                                        const addrinfo& hints,
                                        bool verbose)
{
    return get_hosts(host, &hints, verbose);
}
