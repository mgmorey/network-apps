#include "network-sockets.h"    // Sockets, get_sockets()
#include "network-addrinfo.h"   // copy_addrinfo()
#include "network-hostname.h"   // get_hostname()

#include <string>       // std::string

Network::Sockets Network::get_sockets(std::string& error,
                                      const Hostname& host,
                                      const Service& service,
                                      const struct addrinfo& hints)
{
    Hostname node(host);

    if (host.empty() && service.empty()) {
        HostnameResult hostname_result = get_hostname();
        node = hostname_result.first;
    }

    return get_addrinfo<Sockets>(error, node, service, &hints);
}
