#include "network/get-sockaddr.h"       // Byte, SockAddr,
                                        // get_sockaddr(),
                                        // sockaddr_storage,
                                        // sockaddr_un
#include "network/get-bytespan.h"       // get_bytespan(), std::size_t

#include <algorithm>    // std::max()

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
    const auto data {static_cast<Byte>(0)};
    const auto size {get_capacity()};
    return {size, data};
}
