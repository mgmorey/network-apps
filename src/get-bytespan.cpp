#include "network/get-bytespan.h"       // ByteSpan, get_bytespan(),
                                        // sockaddr, std::size_t

auto Network::get_bytespan(const sockaddr* sa,
                           std::size_t size) -> Network::ByteSpan
{
    const auto *const data {reinterpret_cast<const Byte*>(sa)};  // NOLINT
    return {data, size};
}

#ifndef _WIN32

auto Network::get_bytespan(const sockaddr_un* sun,
                           std::size_t size) -> Network::ByteSpan
{
    const auto *const data {reinterpret_cast<const Byte*>(sun)};  // NOLINT
    return {data, size};
}

#endif
