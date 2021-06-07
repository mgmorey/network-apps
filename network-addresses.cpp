#include "network-addresses.h"
#include "network-addrinfo.h"

Network::Addresses Network::get_addresses(const Network::Hostname& node,
                                          const struct addrinfo* hints)
{
    return get_addrinfo<Addresses>(node, "", hints);
}

Network::Addresses Network::get_addresses(const Network::Hostname& node,
                                          const struct addrinfo& hints)
{
    return get_addresses(node, &hints);
}
