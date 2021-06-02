#include "network-sockets.h"
#include "network-addrinfo.h"
#include "network-socket.h"

#ifdef _WIN32
#include <winsock2.h>		// closesocket(), connect(),
                                // gethostname(), socket()
#include <ws2tcpip.h>		// NI_MAXHOST, struct addrinfo,
                                // getaddrinfo(), getnameinfo(),
                                // socklen_t
#else
#include <netdb.h>		// NI_MAXHOST, struct addrinfo,
                                // getaddrinfo(), getnameinfo()
#include <sys/socket.h>		// AF_UNSPEC, connect(), socket(),
                                // socklen_t
#include <unistd.h>		// close(), gethostname()
#endif

#include <cassert>		// assert()
#include <cerrno>		// errno
#include <cstdlib>		// free()
#include <cstring>		// std::memcpy(), strdup(),
                                // std::strerror()
#include <iostream>		// std::cerr, std::endl

Sockets get_sockets(const Hostname& node,
                    const Service& service,
                    const struct addrinfo* hints)
{
    Sockets result;
    copy_addrinfo(result, node, service, hints);
    return result;
}

Sockets get_sockets(const Hostname& node,
                    const Service& service,
                    const struct addrinfo& hints)
{
    return get_sockets(node, service, &hints);
}
