#include "network-addresses.h"	// Addresses, get_addresses()
#include "network-addrinfo.h"   // get_addrinfo()
#include "network-hostname.h"   // get_hostname()

Network::Addresses Network::get_addresses(const Network::Hostname& node,
                                          const Network::Service& service,
                                          const struct addrinfo& hints)
{
    return get_addrinfo<Addresses>(node.empty() ? get_hostname() : node,
                                   service,
                                   &hints);
}
