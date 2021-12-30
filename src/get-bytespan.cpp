#include "network/get-bytespan.h"       // ByteSpan, get_bytespan(),
                                        // sockaddr, std::size_t

auto Network::get_bytespan(const sockaddr* sa,
                           std::size_t size) -> Network::ByteSpan
{
    const void* pointer = sa;
    const auto* const data {static_cast<const Byte*>(pointer)};
    return {data, size};
}

#ifndef _WIN32

auto Network::get_bytespan(const sockaddr_un* sun,
                           std::size_t size) -> Network::ByteSpan
{
    const void* pointer = sun;
    const auto* const data {static_cast<const Byte*>(pointer)};
    return {data, size};
}

#endif
