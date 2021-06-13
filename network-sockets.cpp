#include "network-sockets.h"    // Sockets, get_sockets()
#include "network-addrinfo.h"   // copy_addrinfo()
#include "network-hostname.h"   // get_hostname()

Network::Sockets Network::get_sockets(const Hostname& hostname,
                                      const Service& service,
                                      const struct addrinfo& hints)
{
    Hostname node(hostname);

    if (hostname.empty() && service.empty()) {
        HostnameResult hostname_result = get_hostname();
        node = hostname_result.first;
    }

    return get_addrinfo<Sockets>(node, service, &hints);
}
