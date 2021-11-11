#include "network-sockname.h"   // Fd, SockAddrResult, get_sockname()
#include "network-name.h"       // get_name()

#ifdef _WIN32
#include <winsock2.h>       // getsockname()
#else
#include <sys/socket.h>     // getsockname()
#endif

using namespace std::literals::string_literals;

static const Network::GetNameBinding binding {::getsockname, "getsockname"s};

Network::SockAddrResult Network::get_sockname(Fd fd, bool verbose)
{
    return get_name(binding, fd, verbose);
}
