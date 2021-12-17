#include "network/get-peername.h"   // Fd, SockAddrResult, get_peername()

#ifdef _WIN32
#include <winsock2.h>   // getpeername()
#else
#include <sys/socket.h> // getpeername()
#endif

static const Network::GetNameHandler handler {::getpeername, "getpeername"};

auto Network::get_peername(Fd fd, bool verbose) -> Network::SockAddrResult
{
    return get_name(handler, fd, verbose);
}
