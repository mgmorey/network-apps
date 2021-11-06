#include "network-peername.h"   // Fd, Result, SockAddr,
                                // get_peername()
#include "network-name.h"       // get_name()

#ifdef _WIN32
#include <winsock2.h>   // getpeername()
#else
#include <sys/socket.h> // getpeername()
#endif

std::pair<Network::SockAddr, Network::Result>
Network::get_peername(const Fd& fd, bool verbose)
{
    return get_name(fd, ::getpeername, "getpeername", verbose);
}
