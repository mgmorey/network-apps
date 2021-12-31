#include "network/bind.h"       // Bytes, Endpoint, Fd,
                                // FdResultVector, Hints, Result,
                                // bind(), open()

#ifdef _WIN32
#include <winsock2.h>       // bind()
#else
#include <sys/socket.h>     // bind()
#endif

static constexpr Network::OpenHandler handler {::bind, "bind"};

auto Network::bind(Fd fd,
                   const Bytes& addr,
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
