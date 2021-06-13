#include "network-addresses.h"	// Addresses, Hostname, Service,
                                // struct addrinfo, get_addresses()
#include "network-addrinfo.h"   // get_addrinfo()
#include "network-hostname.h"   // HostnameResult, get_hostname()

#include <string>       // std::string

Network::Addresses Network::get_addresses(std::string& error,
                                          const Network::Hostname& host,
                                          const Network::Service& service,
                                          const struct addrinfo& hints)
{
    Hostname node(host);

    if (host.empty() && service.empty()) {
        HostnameResult hostname_result = get_hostname();
        node = hostname_result.first;
    }

    return get_addrinfo<Addresses>(error, node, service, &hints);
}
