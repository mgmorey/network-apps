#include "network-addresses.h"
#include "network-addrinfo.h"
#include "network-address.h"

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

Network::Addresses Network::get_addresses(const Network::Hostname& node,
                                          const struct addrinfo* hints)
{
    Network::Addresses result;
    copy_addrinfo(result, node, "", hints);
    result.unique();
    return result;
}

Network::Addresses Network::get_addresses(const Network::Hostname& node,
                                          const struct addrinfo& hints)
{
    return get_addresses(node, &hints);
}
