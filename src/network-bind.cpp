#include "network-bind.h"       // Endpoint, Fd, Hints, Result,
                                // SockAddr, SocketResults, bind(),
                                // open()

#ifdef _WIN32
#include <winsock2.h>       // bind()
#else
#include <sys/socket.h>     // bind()
#endif

static const Network::OpenMethod method {::bind, "bind"};

Network::Result Network::bind(Fd fd,
                              const SockAddr& addr,
                              bool verbose)
{
    return open(method, fd, addr, verbose);
}

Network::SocketResults Network::bind(const Endpoint& endpoint,
                                     const Hints* hints,
                                     bool verbose)
{
    return open(method, endpoint, hints, verbose);
}
