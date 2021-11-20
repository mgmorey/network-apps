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

Network::Result Network::bind(Fd fd,
                              const SockAddr& addr,
                              bool verbose)
{
    return open(handler, fd, addr, verbose);
}

Network::FdResultVector Network::bind(const Endpoint& endp,
                                      const Hints* hints,
                                      bool verbose)
{
    return open(handler, endp, hints, verbose);
}
