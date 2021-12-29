#include "network/get-bytespan.h"       // ByteSpan, get_bytespan(),
                                        // sockaddr, std::span

auto Network::get_bytespan(const sockaddr* sa,
                           std::size_t size) -> Network::ByteSpan
{
    const auto *const data {reinterpret_cast<const Byte*>(sa)};  // NOLINT
    return {data, size};
}
