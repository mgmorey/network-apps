#include "network-sockaddr.h"   // SockAddr, sockaddr, socklen_type

#ifdef _WIN32
#include <winsock2.h>   // sockaddr_storage
#else
#include <sys/socket.h> // sockaddr_storage
#include <sys/un.h>     // sockaddr_un
#endif

#include <algorithm>    // std::max()
#include <cassert>      // assert()
#include <cstddef>      // std::byte, std::size_t
#include <cstring>      // std::strncpy()

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
        const auto size {static_cast<SockAddr::size_type>(addr_len)};
        const auto data {reinterpret_cast<const std::byte*>(addr_ptr)};
        addr.assign(data, data + size);
    }

    assert(addr.size() >= static_cast<std::size_t>(addr_len));
    return addr;
}

Network::SockAddr Network::get_sockaddr(const sockaddr_in* addr_ptr,
                                        socklen_type addr_len)
{
    assert(addr_len == sizeof *addr_ptr);
    return get_sockaddr(reinterpret_cast<const sockaddr*>(addr_ptr), addr_len);
}

Network::SockAddr Network::get_sockaddr(const sockaddr_in6* addr_ptr,
                                        socklen_type addr_len)
{
    assert(addr_len == sizeof *addr_ptr);
    return get_sockaddr(reinterpret_cast<const sockaddr*>(addr_ptr), addr_len);
}

#ifndef _WIN32

Network::SockAddr Network::get_sockaddr(const sockaddr_un* addr_ptr,
                                        socklen_type addr_len)
{
    assert(addr_len == sizeof *addr_ptr);
    return get_sockaddr(reinterpret_cast<const sockaddr*>(addr_ptr), addr_len);
}

Network::SockAddr Network::get_sockaddr(const Pathname& path)
{
    sockaddr_un sun = {
#ifdef HAVE_SOCKADDR_SA_LEN
        sizeof sun,
#endif
        AF_UNIX,
        ""
    };
    assert(path.size() < sizeof sun.sun_path);
    std::strncpy(sun.sun_path, path.c_str(), sizeof sun.sun_path);
#ifdef HAVE_SOCKADDR_SA_LEN
    sun.sun_len = SUN_LEN(&sun);
#endif
    return Network::get_sockaddr(&sun, sizeof sun);
}

#endif
