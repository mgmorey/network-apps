#include "network-addresses.h"	// Addresses, get_addresses()
#include "network-addrinfo.h"   // get_addrinfo()
#include "network-hostname.h"   // get_hostname()

Network::Addresses Network::get_addresses(const Network::Hostname& host,
                                          const Network::Service& service,
                                          const struct addrinfo& hints)
{
    Hostname node(host.empty() && service.empty() ? get_hostname() : host);
    return get_addrinfo<Addresses>(node, service, &hints);
}
