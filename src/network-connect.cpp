#include "network-connect.h"    // Endpoint, Fd, Hints, Result,
                                // SockAddr, SocketResults, connect(),
                                // open()

#ifdef _WIN32
#include <winsock2.h>       // connect()
#else
#include <sys/socket.h>     // connect()
#endif

Network::Result Network::connect(const Fd& fd,
                                 const SockAddr& sock_addr,
                                 bool verbose)
{
    return open(fd, sock_addr, ::connect, "connect", verbose);
}

Network::SocketResults Network::connect(const Endpoint& endpoint,
                                        const Hints* hints,
                                        bool verbose)
{
    return open(endpoint, hints, ::connect, "connect", verbose);
}
