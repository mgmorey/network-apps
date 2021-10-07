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

static std::size_t get_size(const Network::SockAddr& addr)
{
    const auto family {Network::get_family(addr)};

    switch (family) {
    case AF_INET:
        return sizeof(sockaddr_in);
        break;
    case AF_INET6:
        return sizeof(sockaddr_in6);
        break;
#ifndef _WIN32
    case AF_UNIX:
        return sizeof(sockaddr_un);
        break;
#endif
    default:
        return 0;
    }
}

int Network::get_family(const SockAddr& addr)
{
    const auto sa {reinterpret_cast<const sockaddr*>(addr.data())};
    return sa->sa_family;
}

Network::socklen_type Network::get_length(const SockAddr& addr)
{
    const auto sa {reinterpret_cast<const sockaddr*>(addr.data())};

    if (!sa->sa_family) {
        return 0;
    }

#ifdef HAVE_SOCKADDR_SA_LEN
    return sa->sa_len;
#else
    return 0;
#endif
}

const sockaddr* Network::get_pointer(const SockAddr& addr)
{
    return reinterpret_cast<const sockaddr*>(addr.data());
}

sockaddr* Network::get_pointer(SockAddr& addr)
{
    return reinterpret_cast<sockaddr*>(addr.data());
}

Network::SockAddr Network::get_sockaddr(const sockaddr* sa,
                                        std::size_t size)
{
    assert(size ? sa != nullptr : sa == nullptr);
    SockAddr addr;

    if (sa == nullptr) {
        addr.assign(get_capacity(), static_cast<Byte>(0));
        assert(size <= addr.size());
    }
    else {
        const auto data {reinterpret_cast<const Byte*>(sa)};
        addr.assign(data, data + size);
        assert(size == get_size(addr));
        assert(size == addr.size());
    }

    return addr;
}

Network::SockAddr Network::get_sockaddr(const sockaddr_in* sin)
{
    const auto sa {reinterpret_cast<const sockaddr*>(sin)};
    return get_sockaddr(sa, sizeof *sin);
}

Network::SockAddr Network::get_sockaddr(const sockaddr_in6* sin6)
{
    const auto sa {reinterpret_cast<const sockaddr*>(sin6)};
    return get_sockaddr(sa, sizeof *sin6);
}

#ifndef _WIN32

Network::SockAddr Network::get_sockaddr(const sockaddr_un* sun)
{
    const auto sa {reinterpret_cast<const sockaddr*>(sun)};
    return get_sockaddr(sa, sizeof *sun);
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
    return get_sockaddr(&addr);
}

#endif

bool Network::is_valid(const SockAddr& addr)
{
    // const auto family {get_family(addr)};
    const auto length {get_length(addr)};
    const auto size {get_size(addr)};

    if (size < static_cast<std::size_t>(length)) {
        return false;
    }

    if (addr.size() < size) {
        return false;
    }

    return true;
}

Network::SockAddr Network::resize(const SockAddr& addr)
{
    auto size {get_size(addr)};

    if (!size) {
        size = addr.size();
    }

    return addr.substr(0, size);
}
