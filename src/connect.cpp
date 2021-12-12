#include "network/connect.h"    // Endpoint, Fd, FdResultVector,
                                // Hints, Result, SockAddr, connect(),
                                // open()

#ifdef _WIN32
#include <winsock2.h>       // connect()
#else
#include <sys/socket.h>     // connect()
#endif

using namespace std::literals::string_literals;

static const Network::OpenHandler handler {::connect, "connect"s};

auto Network::connect(Fd fd,
                      const SockAddr& addr,
                      bool verbose) -> Network::Result
{
    return open(handler, fd, addr, verbose);
}

auto Network::connect(const Endpoint& endp,
                      const Hints* hints,
                      bool verbose) -> Network::FdResultVector
{
    return open(handler, endp, hints, verbose);
}
