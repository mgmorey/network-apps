#include "network/get-sockaddr.h"       // Bytes, ByteSpan,
                                        // get_sockaddr()

auto Network::get_sockaddr(const ByteSpan& span) -> Network::Bytes
{
    return {span.data(), span.size()};
}
