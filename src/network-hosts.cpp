#include "network-hosts.h"      // HostsResult, Hostname, get_hosts()
#include "network-addrinfo.h"   // get_addrinfo()
#include "network-hostname.h"   // get_hostname()

Network::HostsResult Network::get_hosts(const Hostname& host,
                                        const addrinfo& hints,
                                        bool verbose)
{
    const Hostname node(host.empty() ? get_hostname().first : host);
    return get_addrinfo<HostsResult>(node, "", &hints, true, verbose);
}

Network::HostsResult Network::get_hosts(const Hostname& host,
                                        bool verbose)
{
    const Hostname node(host.empty() ? get_hostname().first : host);
    return get_addrinfo<HostsResult>(node, "", NULL, true, verbose);
}
