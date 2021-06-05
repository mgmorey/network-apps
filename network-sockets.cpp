#include "network-sockets.h"
#include "network-addrinfo.h"
#include "network-socket.h"

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

Network::Sockets Network::get_sockets(const Hostname& node,
                                      const Service& service,
                                      const struct addrinfo* hints)
{
    Sockets result;
    copy_addrinfo(result, node, service, hints);
    return result;
}

Network::Sockets Network::get_sockets(const Hostname& node,
                                      const Service& service,
                                      const struct addrinfo& hints)
{
    return get_sockets(node, service, &hints);
}
