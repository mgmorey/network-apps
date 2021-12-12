#include "network/get-sockname.h"   // Fd, SockAddrResult, get_sockname()

#ifdef _WIN32
#include <winsock2.h>       // getsockname()
#else
#include <sys/socket.h>     // getsockname()
#endif

using namespace std::literals::string_literals;

static const Network::GetNameHandler handler {::getsockname, "getsockname"s};

auto Network::get_sockname(Fd fd, bool verbose) -> Network::SockAddrResult
{
    return get_name(handler, fd, verbose);
}
