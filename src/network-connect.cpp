#include "network/connect.h"    // Endpoint, Fd, Hints, Result,
                                // SockAddr, SocketResults, connect(),
                                // open()

#ifdef _WIN32
#include <winsock2.h>       // connect()
#else
#include <sys/socket.h>     // connect()
#endif

using namespace std::literals::string_literals;

static const Network::OpenHandler handler {::connect, "connect"s};

Network::Result Network::connect(Fd fd,
                                 const SockAddr& addr,
                                 bool verbose)
{
    return open(handler, fd, addr, verbose);
}

Network::SocketResults Network::connect(const Endpoint& endp,
                                        const Hints* hints,
                                        bool verbose)
{
    return open(handler, endp, hints, verbose);
}
