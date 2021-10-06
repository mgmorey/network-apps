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

#ifndef _WIN32
static unsigned char get_length(const sockaddr_un* addr_ptr)
{
#ifdef HAVE_SOCKADDR_SA_LEN
    return static_cast<unsigned char>(SUN_LEN(addr_ptr));
#else
    return static_cast<unsigned char>(sizeof *addr_ptr);
#endif
}
#endif

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
        addr.assign(data, data + size);
    }

    assert(addr.size() >= static_cast<std::size_t>(addr_len));
    return addr;
}

Network::SockAddr Network::get_sockaddr(const sockaddr_in* addr_ptr,
                                        socklen_type addr_len)
{
    const auto ptr {reinterpret_cast<const sockaddr*>(addr_ptr)};
    assert(addr_len == sizeof *addr_ptr);
    return get_sockaddr(ptr, addr_len);
}

Network::SockAddr Network::get_sockaddr(const sockaddr_in6* addr_ptr,
                                        socklen_type addr_len)
{
    const auto ptr {reinterpret_cast<const sockaddr*>(addr_ptr)};
    assert(addr_len == sizeof *addr_ptr);
    return get_sockaddr(ptr, addr_len);
}

#ifndef _WIN32

Network::SockAddr Network::get_sockaddr(const sockaddr_un* addr_ptr,
                                        socklen_type addr_len)
{
    const auto ptr {reinterpret_cast<const sockaddr*>(addr_ptr)};
    assert(addr_len == sizeof *addr_ptr);
    return get_sockaddr(ptr, addr_len);
}

Network::SockAddr Network::get_sockaddr(const Pathname& path)
{
    sockaddr_un sun;
    const auto data {path.c_str()};
    const auto size {sizeof sun.sun_path};
    std::strncpy(sun.sun_path, data, size - 1);
    sun.sun_path[size - 1] = '\0';
    sun.sun_family = AF_UNIX;
#ifdef HAVE_SOCKADDR_SA_LEN
    sun.sun_len = get_length(&sun);
#else
    sun.sun_len = sizeof sun;
#endif
    return get_sockaddr(&sun, sizeof sun);
}

#endif
