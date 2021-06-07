#include "network-sockets.h"    // Sockets
#include "network-addrinfo.h"   // copy_addrinfo()

Network::Sockets Network::get_sockets(const Hostname& node,
                                      const Service& service,
                                      const struct addrinfo& hints)
{
    return get_addrinfo<Sockets>(node, service, &hints);
}
