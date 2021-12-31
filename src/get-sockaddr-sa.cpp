#include "network/get-sockaddr.h"       // Bytes, get_sockaddr(),
                                        // sockaddr
#include "network/get-bytespan.h"       // get_bytespan(),
                                        // std::size_t

auto Network::get_sockaddr(const sockaddr* sa,
                           std::size_t size) -> Network::Bytes
{
    return get_sockaddr(get_bytespan(sa, size));
}
