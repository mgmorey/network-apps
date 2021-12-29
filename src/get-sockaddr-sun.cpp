#include "network/get-sockaddr.h"       // SockAddr, get_sockaddr(),
                                        // sockaddr_un
#include "network/get-bytespan.h"       // get_bytespan()

#ifndef _WIN32

auto Network::get_sockaddr(const sockaddr_un* sun,
                           std::size_t size) -> Network::SockAddr
{
    return get_sockaddr(get_bytespan(sun, size));
}

#endif
