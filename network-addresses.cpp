#include "network-addresses.h"
#include "network-addrinfo.h"

Network::Addresses Network::get_addresses(const Network::Hostname& node,
                                          const Network::Service& service,
                                          const struct addrinfo& hints)
{
    return get_addrinfo<Addresses>(node.empty() ? get_hostname() : node,
                                   service,
                                   &hints);
}
