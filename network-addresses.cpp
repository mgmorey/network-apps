#include "network-addresses.h"	// Addresses, get_addresses()
#include "network-addrinfo.h"   // get_addrinfo()
#include "network-hostname.h"   // get_hostname()

Network::Addresses Network::get_addresses(const Network::Hostname& hostname,
                                          const Network::Service& service,
                                          const struct addrinfo& hints)
{
    Hostname node(hostname);

    if (hostname.empty() && service.empty()) {
        node = get_hostname();
    }

    return get_addrinfo<Addresses>(node, service, &hints);
}
