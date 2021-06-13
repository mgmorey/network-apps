#include "network-addresses.h"	// Addresses, AddressesResult,
                                // Hostname, Service, struct addrinfo,
                                // get_addresses()
#include "network-addrinfo.h"   // get_addrinfo()
#include "network-hostname.h"   // HostnameResult, get_hostname()

#include <string>       // std::string

Network::AddressesResult Network::get_addresses(const Hostname& host,
                                                const Service& service,
                                                const struct addrinfo& hints)
{
    std::string error;
    Hostname node(host);

    if (host.empty() && service.empty()) {
        node = get_hostname().first;
    }

    Addresses addresses(get_addrinfo<Addresses>(error, node, service, &hints));
    return AddressesResult(addresses, error);
}
