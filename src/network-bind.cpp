#include "network-bind.h"       // Endpoint, Fd, Hints, Result,
                                // SockAddr, SocketResults, bind(),
                                // open()

#ifdef _WIN32
#include <winsock2.h>       // bind()
#else
#include <sys/socket.h>     // bind()
#endif

static const Network::OpenMethod method {::bind, "bind"};

Network::Result Network::bind(const Fd& fd,
                              const SockAddr& sock_addr,
                              bool verbose)
{
    return open(method, fd, sock_addr, verbose);
}

Network::SocketResults Network::bind(const Endpoint& endpoint,
                                     const Hints* hints,
                                     bool verbose)
{
    return open(method, endpoint, hints, verbose);
}
