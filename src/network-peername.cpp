#include "network-peername.h"   // Fd, SockAddrResult, get_peername()
#include "network-name.h"       // get_name()

#ifdef _WIN32
#include <winsock2.h>   // getpeername()
#else
#include <sys/socket.h> // getpeername()
#endif

static const Network::GetNameMethod method {::getpeername, "getpeername"};

Network::SockAddrResult Network::get_peername(const Fd& fd, bool verbose)
{
    return get_name(method, fd, verbose);
}
