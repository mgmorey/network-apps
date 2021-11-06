#include "network-sockname.h"   // Fd, Result, SockAddr,
                                // get_sockname()
#include "network-name.h"       // get_name()

#ifdef _WIN32
#include <winsock2.h>       // getsockname()
#else
#include <sys/socket.h>     // getsockname()
#endif

std::pair<Network::SockAddr, Network::Result>
Network::get_sockname(const Fd& fd, bool verbose)
{
    return get_name(fd, ::getsockname, "getsockname", verbose);
}
