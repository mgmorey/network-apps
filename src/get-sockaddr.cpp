#include "network/get-sockaddr.h"       // ByteSpan, SockAddr,
                                        // get_sockaddr()

auto Network::get_sockaddr(const ByteSpan& span) -> Network::SockAddr
{
    return {span.data(), span.size()};
}
