#include "network-sockets.h"    // Sockets, get_sockets()
#include "network-addrinfo.h"   // copy_addrinfo()
#include "network-hostname.h"   // get_hostname()

Network::Sockets Network::get_sockets(const Hostname& host,
                                      const Service& service,
                                      const struct addrinfo& hints)
{
    Hostname node(host.empty() && service.empty() ? get_hostname() : host);
    return get_addrinfo<Sockets>(node, service, &hints);
}
