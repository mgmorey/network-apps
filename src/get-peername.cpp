#include "network/get-peername.h"   // BytesResult, Fd, get_peername()

#ifdef _WIN32
#include <winsock2.h>   // getpeername()
#else
#include <sys/socket.h> // getpeername()
#endif

static constexpr Network::GetNameHandler handler {::getpeername, "getpeername"};

auto Network::get_peername(Fd fd, bool verbose) -> Network::BytesResult
{
    return get_name(handler, fd, verbose);
}
