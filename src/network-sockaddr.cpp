#include "network-sockaddr.h"   // SockAddr, sockaddr, sockaddr_in,
                                // sockaddr_in6, socklen_type

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_LOCAL, AF_UNIX,
                        // sockaddr_storage
#else
#include <sys/socket.h> // AF_INET, AF_INET6, AF_LOCAL, AF_UNIX,
                        // sockaddr_storage
#include <sys/un.h>     // sockaddr_un
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

static std::size_t get_max_size(const Network::SockAddr& sock_addr)
{
    const auto family {Network::get_family(sock_addr)};

    switch (family) {
#ifndef _WIN32
    case AF_UNIX:
        return sizeof(sockaddr_un);
#endif
    case AF_INET:
        return sizeof(sockaddr_in);
    case AF_INET6:
        return sizeof(sockaddr_in6);
    default:
        return 0;
    }
}

static std::size_t get_min_size(const Network::SockAddr& sock_addr)
{
    const auto family {Network::get_family(sock_addr)};

    switch (family) {
#ifndef _WIN32
    case AF_UNIX:
        {
            const auto sun {
                reinterpret_cast<const sockaddr_un*>(sock_addr.data())
            };
            return sizeof *sun - sizeof sun->sun_path;
        }
#endif
    case AF_INET:
        return sizeof(sockaddr_in);
    case AF_INET6:
        return sizeof(sockaddr_in6);
    default:
        return sizeof(sockaddr);
    }
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

#ifndef _WIN32
#ifdef HAVE_SOCKADDR_SA_LEN
static std::size_t get_sun_length(const sockaddr_un* sun)
{
    const auto path_size {strnlen(sun->sun_path, sizeof(sun->sun_path))};
    const auto min_size {sizeof *sun - sizeof sun->sun_path + path_size};
    return std::max(sizeof(sockaddr), min_size);
}
#endif
#endif

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

const sockaddr* Network::get_pointer(const SockAddr& sock_addr)
{
    const auto sa {reinterpret_cast<const sockaddr*>(sock_addr.data())};
    assert(sa != nullptr);
    return sa;
}

sockaddr* Network::get_pointer(SockAddr& sock_addr)
{
    const auto sa {reinterpret_cast<sockaddr*>(sock_addr.data())};
    assert(sa != nullptr);
    return sa;
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
    std::memset(&addr, '\0', sizeof addr);
    const auto path_size {std::min(path.size(), sizeof addr.sun_path - 1)};
    const auto min_size {sizeof addr - sizeof addr.sun_path + path_size};
    std::memcpy(addr.sun_path, path, path_size);
    addr.sun_family = AF_LOCAL;
#ifdef HAVE_SOCKADDR_SA_LEN
    const auto sun_len {std::max(sizeof(sockaddr), min_size)};
    addr.sun_len = sun_len;
    assert(addr.sun_len == get_sun_length(&addr));
#endif
    const auto len {std::max(sizeof(sockaddr), min_size + 1)};
    return get_sockaddr(&addr, len);
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
#ifndef _WIN32
    case AF_UNIX:
        break;
#endif
    case AF_INET:
        break;
    case AF_INET6:
        break;
    default:
        return false;
    }

    const auto max_size {get_max_size(sock_addr)};
    const auto min_size {get_min_size(sock_addr)};

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
                  << "    Minimum size:"
                  << std::right
                  << std::setw(5)
                  << min_size
                  << std::endl
                  << std::left
                  << std::setw(20)
                  << "    Maximum size:"
                  << std::right
                  << std::setw(5)
                  << max_size
                  << std::endl;
    }

    if (!(min_size <= sock_addr.size() && sock_addr.size() <= max_size)) {
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

    if (family == AF_UNIX) {
        if (!(min_size <= sa_len && sa_len <= sock_addr.size())) {
            return false;
        }

#ifndef _WIN32

        const auto sun {reinterpret_cast<const sockaddr_un*>(sock_addr.data())};
        const auto sun_len {get_sun_length(sun)};

        if (verbose) {
            std::cerr << std::left
                      << std::setw(20)
                      << "    Computed length:"
                      << std::right
                      << std::setw(5)
                      << sun_len
                      << std::endl;
        }

        if (!(sa_len == sun_len)) {
            return false;
        }

#endif

    }
    else {
        if (!(sa_len == sock_addr.size())) {
            return false;
        }
    }

#endif

    return true;
}
