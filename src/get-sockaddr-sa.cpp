#include "network/get-sockaddr.h"       // Byte, ByteSpan, SockAddr,
                                        // get_sockaddr(), sockaddr
#include "network/get-bytespan.h"       // get_bytespan(), std::span

#ifdef _WIN32
#include <winsock2.h>       // sockaddr_storage
#else
#include <sys/socket.h>     // sockaddr_storage
#include <sys/un.h>         // sockaddr_un
#endif

#include <algorithm>    // std::max()
#include <cstddef>      // std::size_t

static constexpr auto get_capacity() -> Network::SockAddr::size_type
{
    constexpr auto storage_size {sizeof(sockaddr_storage)};
#ifdef _WIN32
    constexpr auto unix_size {static_cast<std::size_t>(0)};
#else
    constexpr auto unix_size {sizeof(sockaddr_un)};
#endif
    return std::max(storage_size, unix_size);
}

auto Network::get_sockaddr() -> Network::SockAddr
{
    return {get_capacity(), static_cast<Byte>(0)};
}

auto Network::get_sockaddr(const sockaddr* sa,
                           std::size_t size) -> Network::SockAddr
{
    return get_sockaddr(get_bytespan(sa, size));
}
