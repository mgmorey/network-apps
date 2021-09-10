#include "network-hosts.h"      // Hostname, HostsResult, get_hosts()
#include "network-addrinfo.h"   // AddrInfo, operator<<()
#include "network-hostname.h"   // get_hostname()

Network::HostsResult Network::get_hosts(const Hostname& hostname,
                                        const Hints* hints,
                                        bool verbose)
{
    const auto endpoint(Endpoint(get_hostname(hostname), ""));
    return AddrInfo::get<HostsResult>(endpoint, hints, verbose);
}
