#include "network/get-sockaddr.h"       // SockAddr, get_sockaddr(),
                                        // sockaddr

#ifdef _WIN32
#include <winsock2.h>       // sockaddr_storage
#else
#include <sys/socket.h>     // sockaddr_storage
#include <sys/un.h>         // sockaddr_un
#endif

#include <algorithm>    // std::max()
#include <array>        // std::array
#include <cstddef>      // std::size_t
#include <span>         // std::as_bytes(), std::span

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

auto Network::get_sockaddr(const sockaddr* psa,
                           std::size_t size) -> Network::SockAddr
{
    const std::array<sockaddr, 1> sa {*psa};
    const auto bytes {std::as_bytes(std::span(sa))};
    SockAddr addr {bytes.data(), bytes.size()};

    if (size != 0) {
        addr.resize(size);
    }

    return addr;
}
