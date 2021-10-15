#include "network-bind.h"       // Endpoint, Fd, Hints, Result,
                                // Socket, SockAddr, SocketResult,
                                // fd_null, open(), operator<<()
#include "network-open.h"       // open()

#ifdef _WIN32
#include <winsock2.h>       // bind()
#else
#include <sys/socket.h>     // bind()
#endif

Network::Result Network::bind(Fd fd, const SockAddr& sock_addr, bool verbose)
{
    return open(fd, sock_addr, ::bind, "bind", verbose);
}

std::vector<Network::SocketResult> Network::bind(const Endpoint& endpoint,
                                                 const Hints* hints,
                                                 bool verbose)
{
    return open(endpoint, hints, ::bind, "bind", verbose);
}
