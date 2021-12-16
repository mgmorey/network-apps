#include "network/get-sockaddr.h"       // SockAddr, get_sockaddr(),
                                        // sockaddr, sockaddr_in,
                                        // sockaddr_in6
#include "network/get-sun-length.h"     // SockAddr, get_sun_length()

#ifdef _WIN32
#include <winsock2.h>   // AF_LOCAL, sockaddr_storage
#else
#include <sys/socket.h> // AF_LOCAL, sockaddr_storage
#include <sys/un.h>     // sockaddr_un
#endif

#include <algorithm>    // std::max(), std::min()
#include <cassert>      // assert()
#include <cstddef>      // std::size_t
#include <cstring>      // std::memcpy(), std::memset()

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

auto Network::get_sockaddr(const sockaddr* sa,
                           std::size_t size) -> Network::SockAddr
{
    assert(size ? sa != nullptr : sa == nullptr);
    SockAddr addr;

    if (sa == nullptr) {
        addr.assign(get_capacity(), static_cast<Byte>(0));
    }
    else {
        const auto *const data {reinterpret_cast<const Byte*>(sa)};
        addr.assign(data, data + size);
    }

    return addr;
}

auto Network::get_sockaddr(const sockaddr_in* sin) -> Network::SockAddr
{
    const auto *const sa {reinterpret_cast<const sockaddr*>(sin)};
    return get_sockaddr(sa, sizeof *sin);
}

auto Network::get_sockaddr(const sockaddr_in6* sin6) -> Network::SockAddr
{
    const auto *const sa {reinterpret_cast<const sockaddr*>(sin6)};
    return get_sockaddr(sa, sizeof *sin6);
}

#ifndef _WIN32

auto Network::get_sockaddr(const sockaddr_un* sun,
                           std::size_t size) -> Network::SockAddr
{
    const auto *const sa {reinterpret_cast<const sockaddr*>(sun)};
    return get_sockaddr(sa, size != 0 ? size : sizeof(sockaddr_un));
}

auto Network::get_sockaddr(const Pathname& path) -> Network::SockAddr
{
    sockaddr_un addr {};
    std::memset(&addr, '\0', sizeof addr);
    const auto path_size {std::min(path.size(), sizeof addr.sun_path - 1)};
    const auto min_size {sizeof addr - sizeof addr.sun_path + path_size};
    std::memcpy(addr.sun_path, path, path_size);
    addr.sun_family = AF_LOCAL;
#ifdef HAVE_SOCKADDR_SA_LEN
    const auto sun_len {std::max(sizeof(sockaddr), min_size)};
    addr.sun_len = sun_len;
    assert(addr.sun_len == get_sun_length(&addr, sizeof addr));
#endif
    const auto len {std::max(sizeof(sockaddr), min_size + 1)};
    return get_sockaddr(&addr, len);
}

#endif
