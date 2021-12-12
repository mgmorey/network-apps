#include "network/bind.h"       // Endpoint, Fd, FdResultVector,
                                // Hints, Result, SockAddr, bind(),
                                // open()

#ifdef _WIN32
#include <winsock2.h>       // bind()
#else
#include <sys/socket.h>     // bind()
#endif

using namespace std::literals::string_literals;

static const Network::OpenHandler handler {::bind, "bind"s};

auto Network::bind(Fd fd,
                   const SockAddr& addr,
                   bool verbose) -> Network::Result
{
    return open(handler, fd, addr, verbose);
}

auto Network::bind(const Endpoint& endp,
                   const Hints* hints,
                   bool verbose) -> Network::FdResultVector
{
    return open(handler, endp, hints, verbose);
}
