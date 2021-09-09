#include "network-hosts.h"      // Hostname, HostsResult, get_hosts()
#include "network-addrinfo.h"   // AddrInfo, operator<<()
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
                                        const Hints* hints,
                                        bool verbose)
{
    HostsResult result;
    const auto node(get_node(host));
    auto it(std::back_inserter(result.first));
    result.second = AddrInfo::insert(node, "", hints, verbose, it);
    return result;
}
