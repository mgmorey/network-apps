#include "network-addresses.h"	// AddressesResult, Hostname, Service,
                                // struct addrinfo, get_addresses()
#include "network-addrinfo.h"   // get_addrinfo()
#include "network-hostname.h"   // get_hostname()

Network::AddressesResult Network::get_addresses(const Hostname& host,
                                                const addrinfo& hints,
                                                bool verbose)
{
    Hostname node(host);

    if (host.empty()) {
        node = get_hostname().first;
    }

    return get_addrinfo<AddressesResult>(node, "", &hints, true, verbose);
}

Network::AddressesResult Network::get_addresses(const Hostname& host,
                                                bool verbose)
{
    Hostname node(host);

    if (host.empty()) {
        node = get_hostname().first;
    }

    return get_addrinfo<AddressesResult>(node, "", NULL, true, verbose);
}
