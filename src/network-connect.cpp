#include "network-connect.h"    // Endpoint, Fd, Hints, Result,
                                // SockAddr, SocketResults, connect(),
                                // open()

#ifdef _WIN32
#include <winsock2.h>       // connect()
#else
#include <sys/socket.h>     // connect()
#endif

using namespace std::literals::string_literals;

static const Network::OpenBinding binding {::connect, "connect"s};

Network::Result Network::connect(Fd fd,
                                 const SockAddr& addr,
                                 bool verbose)
{
    return open(binding, fd, addr, verbose);
}

Network::SocketResults Network::connect(const Endpoint& endp,
                                        const Hints* hints,
                                        bool verbose)
{
    return open(binding, endp, hints, verbose);
}
