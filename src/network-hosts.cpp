#include "network-hosts.h"      // HostsResult, Hostname, Service,
                                // struct addrinfo, get_hosts()
#include "network-addrinfo.h"   // get_addrinfo()
#include "network-hostname.h"   // get_hostname()

Network::HostsResult Network::get_hosts(const Hostname& host,
                                        const addrinfo& hints,
                                        bool verbose)
{
    Hostname node(host);

    if (host.empty()) {
        node = get_hostname().first;
    }

    return get_addrinfo<HostsResult>(node, "", &hints, true, verbose);
}

Network::HostsResult Network::get_hosts(const Hostname& host,
                                        bool verbose)
{
    Hostname node(host);

    if (host.empty()) {
        node = get_hostname().first;
    }

    return get_addrinfo<HostsResult>(node, "", NULL, true, verbose);
}
