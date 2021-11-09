#include "network-connect.h"    // Endpoint, Fd, Hints, Result,
                                // SockAddr, SocketResults, connect(),
                                // open()

#ifdef _WIN32
#include <winsock2.h>       // connect()
#else
#include <sys/socket.h>     // connect()
#endif

static const Network::OpenMethod method {::connect, "connect"};

Network::Result Network::connect(Fd fd,
                                 const SockAddr& sock_addr,
                                 bool verbose)
{
    return open(method, fd, sock_addr, verbose);
}

Network::SocketResults Network::connect(const Endpoint& endpoint,
                                        const Hints* hints,
                                        bool verbose)
{
    return open(method, endpoint, hints, verbose);
}
