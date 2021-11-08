#include "network-peername.h"   // Fd, SockAddrResult, get_peername()
#include "network-name.h"       // get_name()

#ifdef _WIN32
#include <winsock2.h>   // getpeername()
#else
#include <sys/socket.h> // getpeername()
#endif

Network::SockAddrResult Network::get_peername(const Fd& fd, bool verbose)
{
    return get_name(fd, ::getpeername, "getpeername", verbose);
}
