#include "network-hosts.h"      // HostsResult, Hostname
#include "network-addrinfo.h"   // insert_addrinfo()
#include "network-hostname.h"   // get_hostname()

#include <iterator>     // std::back_inserter()

Network::HostsResult Network::get_hosts(const Hostname& host,
                                        const addrinfo* hints,
                                        bool verbose)
{
    HostsResult result;
    const Hostname node(host.empty() ? get_hostname().first : host);
    result.second = insert_addrinfo(std::back_inserter(result.first),
                                    node, "", hints, verbose);
    return result;
}

Network::HostsResult Network::get_hosts(const Hostname& host,
                                        const addrinfo& hints,
                                        bool verbose)
{
    return get_hosts(host, &hints, verbose);
}
