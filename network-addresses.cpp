#include "network-addresses.h"	// Addresses, Hostname, Service,
                                // struct addrinfo, get_addresses()
#include "network-addrinfo.h"   // get_addrinfo()
#include "network-hostname.h"   // HostnameResult, get_hostname()

Network::Addresses Network::get_addresses(const Network::Hostname& hostname,
                                          const Network::Service& service,
                                          const struct addrinfo& hints)
{
    Hostname node(hostname);

    if (hostname.empty() && service.empty()) {
        HostnameResult hostname_result = get_hostname();
        node = hostname_result.first;
    }

    return get_addrinfo<Addresses>(node, service, &hints);
}
