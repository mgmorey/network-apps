#include "network-addresses.h"	// AddressesResult, Hostname, Service,
                                // struct addrinfo, get_addresses()
#include "network-addrinfo.h"   // get_addrinfo()
#include "network-hostname.h"   // get_hostname()

Network::AddressesResult Network::get_addresses(const Hostname& host,
                                                const Service& service,
                                                const struct addrinfo& hints)
{
    Hostname node(host);

    if (host.empty() && service.empty()) {
        node = get_hostname().first;
    }

    return get_addrinfo<AddressesResult>(node, service, &hints);
}
