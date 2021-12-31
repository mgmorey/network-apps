#include "network/get-sockname.h"   // BytesResult, Fd, get_sockname()

#ifdef _WIN32
#include <winsock2.h>       // getsockname()
#else
#include <sys/socket.h>     // getsockname()
#endif

static constexpr Network::GetNameHandler handler {::getsockname, "getsockname"};

auto Network::get_sockname(Fd fd, bool verbose) -> Network::BytesResult
{
    return get_name(handler, fd, verbose);
}
