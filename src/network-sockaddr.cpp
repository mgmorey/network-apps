#include "network-sockaddr.h"   // SockAddr, sockaddr, socklen_type
#include "network-unix.h"       // SUN_LEN(), sockaddr_un

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_LOCAL, AF_UNIX,
                        // sockaddr_storage
#else
#include <sys/socket.h> // AF_INET, AF_INET6, AF_LOCAL, AF_UNIX,
                        // sockaddr_storage
#endif

#include <algorithm>    // std::max()
#include <cassert>      // assert()
#include <cstddef>      // std::byte, std::size_t
#include <cstring>      // std::memset(), std::strlen(),
                        // std::strncpy()

static constexpr Network::SockAddr::size_type get_capacity()
{
    constexpr auto storage_size {sizeof(sockaddr_storage)};
#ifdef _WIN32
    constexpr auto unix_size {static_cast<std::size_t>(0)};
#else
    constexpr auto unix_size {sizeof(sockaddr_un)};
#endif
    return std::max(storage_size, unix_size);
}

int Network::get_family(const SockAddr& addr)
{
    const auto sa {reinterpret_cast<const sockaddr*>(addr.data())};
    return sa->sa_family;
}

int Network::get_family(SockAddr& addr)
{
    const auto sa {reinterpret_cast<sockaddr*>(addr.data())};
    return sa->sa_family;
}

Network::socklen_type Network::get_length(const SockAddr& addr)
{
    return static_cast<socklen_type>(addr.size());
}

Network::socklen_type Network::get_length(SockAddr& addr)
{
    return static_cast<socklen_type>(addr.size());
}

const sockaddr* Network::get_pointer(const SockAddr& addr)
{
    return reinterpret_cast<const sockaddr*>(addr.data());
}

sockaddr* Network::get_pointer(SockAddr& addr)
{
    return reinterpret_cast<sockaddr*>(addr.data());
}

Network::SockAddr Network::get_sockaddr(const sockaddr* addr_ptr,
                                        socklen_type addr_len)
{
    assert(addr_len ? addr_ptr != nullptr : addr_ptr == nullptr);
    SockAddr addr;

    if (addr_ptr == nullptr) {
        addr.assign(get_capacity(), static_cast<std::byte>(0));
    }
    else {
        const auto data {reinterpret_cast<const std::byte*>(addr_ptr)};
        const auto size {static_cast<SockAddr::size_type>(addr_len)};
        assert(addr_len > sizeof *addr_ptr);
        addr.assign(data, data + size);
    }

    assert(static_cast<std::size_t>(addr_len) <= addr.size());
    return addr;
}

Network::SockAddr Network::get_sockaddr(const sockaddr_in* addr_ptr,
                                        socklen_type addr_len)
{
    const auto sa {reinterpret_cast<const sockaddr*>(addr_ptr)};
    assert(addr_len == sizeof(sockaddr_in));
    return get_sockaddr(sa, addr_len);
}

Network::SockAddr Network::get_sockaddr(const sockaddr_in6* addr_ptr,
                                        socklen_type addr_len)
{
    const auto sa {reinterpret_cast<const sockaddr*>(addr_ptr)};
    assert(addr_len == sizeof(sockaddr_in6));
    return get_sockaddr(sa, addr_len);
}

#ifndef _WIN32

Network::SockAddr Network::get_sockaddr(const sockaddr_un* addr_ptr,
                                        socklen_type addr_len)
{
    const auto sa {reinterpret_cast<const sockaddr*>(addr_ptr)};
    assert(addr_len == sizeof(sockaddr_un));
    return get_sockaddr(sa, addr_len);
}

Network::SockAddr Network::get_sockaddr(const Pathname& path)
{
    sockaddr_un addr;
    std::memset(&addr, '\0', sizeof addr);
    std::strncpy(addr.sun_path, path.c_str(), sizeof addr.sun_path - 1);
    addr.sun_path[sizeof addr.sun_path - 1] = '\0';
    addr.sun_family = AF_LOCAL;
#ifdef HAVE_SOCKADDR_SA_LEN
    addr.sun_len = SUN_LEN(&addr);
#endif
    assert(std::strlen(addr.sun_path) == path.size());
    return get_sockaddr(&addr, sizeof addr);
}

#endif

Network::SockAddr Network::resize(const SockAddr& addr)
{
    const auto family {get_family(addr)};
    std::size_t size {};

    switch (family) {
    case AF_INET:
        size = sizeof(sockaddr_in);
        break;
    case AF_INET6:
        size = sizeof(sockaddr_in6);
        break;
#ifndef _WIN32
    case AF_UNIX:
        size = sizeof(sockaddr_un);
        break;
#endif
    default:
        size = addr.size();
    }

    return size == addr.size() ? addr : addr.substr(0, size);
}
