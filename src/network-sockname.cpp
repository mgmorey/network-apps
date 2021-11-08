#include "network-sockname.h"   // Fd, SockAddrResult, get_sockname()
#include "network-name.h"       // get_name()

#ifdef _WIN32
#include <winsock2.h>       // getsockname()
#else
#include <sys/socket.h>     // getsockname()
#endif

Network::SockAddrResult Network::get_sockname(const Fd& fd, bool verbose)
{
    return get_name(fd, ::getsockname, "getsockname", verbose);
}
