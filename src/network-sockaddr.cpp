#include "network-sockaddr.h"   // SockAddr, sockaddr, sockaddr_in,
                                // sockaddr_in6, socklen_type
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
#include <iomanip>      // std::left, std::right, std::setw()
#include <iostream>     // std::cerr, std::endl
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

static std::size_t get_sa_length(const Network::SockAddr& sock_addr)
{
#ifdef HAVE_SOCKADDR_SA_LEN
    const auto sa {reinterpret_cast<const sockaddr*>(sock_addr.data())};
    assert(sa != nullptr);

    if (sock_addr.empty()) {
        return 0;
    }

    return sa->sa_len;
#else
    static_cast<void>(sock_addr);
    return 0;
#endif
}

int Network::get_family(const SockAddr& sock_addr)
{
    const auto sa {reinterpret_cast<const sockaddr*>(sock_addr.data())};
    assert(sa != nullptr);

    if (sock_addr.empty()) {
        return 0;
    }

    return sa->sa_family;
}

Network::socklen_type Network::get_length(const SockAddr& sock_addr)
{
    const auto length {static_cast<std::size_t>(get_sa_length(sock_addr))};
    return static_cast<socklen_type>(length ? length : sock_addr.size());
}

std::size_t Network::get_max_size(const Network::SockAddr& sock_addr)
{
    const auto family {Network::get_family(sock_addr)};

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

const sockaddr* Network::get_pointer(const SockAddr& sock_addr)
{
    const auto ptr {reinterpret_cast<const sockaddr*>(sock_addr.data())};
    assert(ptr != nullptr);
    return ptr;
}

sockaddr* Network::get_pointer(SockAddr& sock_addr)
{
    const auto ptr {reinterpret_cast<sockaddr*>(sock_addr.data())};
    assert(ptr != nullptr);
    return ptr;
}

Network::SockAddr Network::get_sockaddr(const sockaddr* sa,
                                        std::size_t size)
{
    assert(size ? sa != nullptr : sa == nullptr);
    SockAddr sock_addr;

    if (sa == nullptr) {
        sock_addr.assign(get_capacity(), static_cast<Byte>(0));
    }
    else {
        const auto addr_data {reinterpret_cast<const Byte*>(sa)};
        sock_addr.assign(addr_data, addr_data + size);
    }

    return sock_addr;
}

#ifndef _WIN32

Network::SockAddr Network::get_sockaddr(const sockaddr_un* sun,
                                        std::size_t size)
{
    const auto sa {reinterpret_cast<const sockaddr*>(sun)};
    return get_sockaddr(sa, size);
}

Network::SockAddr Network::get_sockaddr(const Pathname& path)
{
    sockaddr_un addr;
    const auto path_len {std::min(path.length(), sizeof addr.sun_path - 1)};
    const auto addr_len {sizeof addr - sizeof addr.sun_path + path_len};
    std::memset(&addr, '\0', sizeof addr);
#ifdef HAVE_SOCKADDR_SA_LEN
    addr.sun_len = addr_len;
#endif
    addr.sun_family = AF_LOCAL;
    std::memcpy(addr.sun_path, path, path_len);
    return get_sockaddr(&addr, addr_len + 1);
}

#endif

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

bool Network::is_valid(const SockAddr& sock_addr, bool verbose)
{
    if (verbose) {
        std::cerr << "Validating socket address: "
                  << sock_addr
                  << std::endl;
    }

    if (sock_addr.empty()) {
        return false;
    }

    const auto family {get_family(sock_addr)};

    if (verbose) {
        std::cerr << std::left
                  << std::setw(20)
                  << "    Family:"
                  << std::right
                  << std::setw(5)
                  << family
                  << std::endl;
    }

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

    const auto max_size {get_max_size(sock_addr)};

    if (verbose) {
        std::cerr << std::left
                  << std::setw(20)
                  << "    Actual size:"
                  << std::right
                  << std::setw(5)
                  << sock_addr.size()
                  << std::endl
                  << std::left
                  << std::setw(20)
                  << "    Maximum size:"
                  << std::right
                  << std::setw(5)
                  << max_size
                  << std::endl;
    }

    if (sock_addr.size() > max_size) {
        return false;
    }

#ifdef HAVE_SOCKADDR_SA_LEN
    const auto sa_len {static_cast<std::size_t>(get_sa_length(sock_addr))};

    if (verbose) {
        std::cerr << std::left
                  << std::setw(20)
                  << "    Stored length:"
                  << std::right
                  << std::setw(5)
                  << sa_len
                  << std::endl;
    }

    switch (family) {
    case AF_INET:
        if (sa_len != sizeof(sockaddr_in)) {
            return false;
        }
        break;
    case AF_INET6:
        if (sa_len != sizeof(sockaddr_in6)) {
            return false;
        }
        break;
#ifndef _WIN32
    case AF_UNIX:
    {
        const auto sun_pointer {
            reinterpret_cast<const sockaddr_un*>(sock_addr.data())
        };
        const auto sun_len {SUN_LEN(sun_pointer)};

        if (verbose) {
            std::cerr << std::left
                      << std::setw(20)
                      << "    Computed length:"
                      << std::right
                      << std::setw(5)
                      << sun_len
                      << std::endl;
        }

        if (sa_len < sun_len) {
            return false;
        }
        break;
    }
#endif
    }
#endif

    return true;
}
