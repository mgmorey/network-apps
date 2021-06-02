#include "network-addresses.h"
#include "network-addrinfo.h"
#include "network-address.h"

#ifdef _WIN32
#include <ws2tcpip.h>		// struct addrinfo
#else
#include <netdb.h>		// struct addrinfo
#endif

Addresses get_addresses(const Hostname& node,
                        const struct addrinfo* hints)
{
    Addresses result;
    copy_addrinfo(result, node, "", hints);
    result.unique();
    return result;
}

Addresses get_addresses(const Hostname& node,
                        const struct addrinfo& hints)
{
    return get_addresses(node, &hints);
}
