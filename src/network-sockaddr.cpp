#include "network-sockaddr.h"   // SockAddr, sockaddr, socklen_type
#include "network-unix.h"       // SUN_LEN(), sockaddr_un

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_LOCAL, AF_UNIX,
                        // sockaddr_storage
#else
#include <sys/socket.h> // AF_INET, AF_INET6, AF_LOCAL, AF_UNIX,
                        // sockaddr_storage
#endif

#include <algorithm>    // std::max(), std::min()
#include <cassert>      // assert()
#include <cstddef>      // std::byte, std::size_t
#include <cstring>      // std::memcpy(), std::memset()

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

static std::size_t get_sa_length(const Network::SockAddr& addr)
{
    const auto sa {reinterpret_cast<const sockaddr*>(addr.data())};
    assert(sa != nullptr);

    if (addr.empty()) {
        return 0;
    }

#ifdef HAVE_SOCKADDR_SA_LEN
    return sa->sa_len;
#else
    return 0;
#endif
}

int Network::get_family(const SockAddr& addr)
{
    const auto sa {reinterpret_cast<const sockaddr*>(addr.data())};
    assert(sa != nullptr);

    if (addr.empty()) {
        return 0;
    }

    return sa->sa_family;
}

Network::socklen_type Network::get_length(const SockAddr& addr)
{
    const auto length {static_cast<std::size_t>(get_sa_length(addr))};
    return static_cast<socklen_type>(length ? length : addr.size());
}

std::size_t Network::get_max_size(const Network::SockAddr& addr)
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

const sockaddr* Network::get_pointer(const SockAddr& addr)
{
    const auto ptr {reinterpret_cast<const sockaddr*>(addr.data())};
    assert(ptr != nullptr);
    return ptr;
}

sockaddr* Network::get_pointer(SockAddr& addr)
{
    const auto ptr {reinterpret_cast<sockaddr*>(addr.data())};
    assert(ptr != nullptr);
    return ptr;
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
        assert(size == get_max_size(addr));
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
    const std::string string {path};
    const auto length {std::min(string.size(), sizeof addr.sun_path - 1)};
    std::memset(&addr, '\0', sizeof addr);
    std::memcpy(addr.sun_path, string.data(), length);
#ifdef HAVE_SOCKADDR_SA_LEN
    addr.sun_len = SUN_LEN(&addr);
#endif
    addr.sun_family = AF_LOCAL;
    return get_sockaddr(&addr);
}

#endif

bool Network::is_valid(const SockAddr& addr)
{
    if (addr.empty()) {
        return false;
    }

    const auto family {get_family(addr)};

    switch (family) {
    case AF_INET:
        break;
    case AF_INET6:
        break;
#ifndef _WIN32
    case AF_UNIX:
        break;
#endif
    default:
        return false;
    }

    const auto max_size {get_max_size(addr)};

    if (addr.size() > max_size) {
        return false;
    }

#ifdef HAVE_SOCKADDR_SA_LEN
    const auto sa_length {static_cast<std::size_t>(get_sa_length(addr))};

    if (!sa_length) {
        return false;
    }

    if (sa_length > max_size) {
        return false;
    }

    if (addr.size() > sa_length) {
        return false;
    }
#endif

    return true;
}
