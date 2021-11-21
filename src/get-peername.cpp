#include "network/get-peername.h"   // Fd, SockAddrResult, get_peername()

#ifdef _WIN32
#include <winsock2.h>   // getpeername()
#else
#include <sys/socket.h> // getpeername()
#endif

using namespace std::literals::string_literals;

static const Network::GetNameHandler handler {::getpeername, "getpeername"s};

Network::SockAddrResult Network::get_peername(Fd fd, bool verbose)
{
    return get_name(handler, fd, verbose);
}
