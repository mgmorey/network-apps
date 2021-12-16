#include "network/get-sockaddr.h"       // SockAddr, sockaddr,
                                        // sockaddr_in, sockaddr_in6,
                                        // sock_len_type
#include "network/family-type.h"        // family_type

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
#include <cstddef>      // std::size_t
#include <cstring>      // std::memcpy(), std::memset()
#include <cstring>      // strnlen()
#include <iomanip>      // std::left, std::right, std::setw()
#include <iostream>     // std::cerr, std::endl

static const auto key_width {20};
static const auto value_width {5};

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

static auto get_family(const Network::SockAddr& addr) -> Network::family_type
{
    const auto *const sa {reinterpret_cast<const sockaddr*>(addr.data())};
    assert(sa != nullptr);

    if (addr.empty()) {
        return 0;
    }

    return sa->sa_family;
}

static auto get_max_size(const Network::SockAddr& addr) -> std::size_t
{
    const auto family {get_family(addr)};

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

static auto get_min_size(const Network::SockAddr& addr) -> std::size_t
{
    const auto family {get_family(addr)};

    switch (family) {
#ifndef _WIN32
    case AF_UNIX:
        return offsetof(sockaddr_un, sun_path);
#endif
    case AF_INET:
        return sizeof(sockaddr_in);
    case AF_INET6:
        return sizeof(sockaddr_in6);
    default:
        return sizeof(sockaddr);
    }
}

static auto get_sa_length(const Network::SockAddr& addr) -> std::size_t
{
#ifdef HAVE_SOCKADDR_SA_LEN
    const auto *const sa {reinterpret_cast<const sockaddr*>(addr.data())};
    assert(sa != nullptr);

    if (addr.empty()) {
        return 0;
    }

    return sa->sa_len;
#else
    static_cast<void>(addr);
    return 0;
#endif
}

auto Network::get_length(const SockAddr& addr) -> Network::sock_len_type
{
    const auto length {static_cast<std::size_t>(get_sa_length(addr))};
    return static_cast<sock_len_type>(length != 0 ? length : addr.size());
}

auto Network::get_pointer(const SockAddr& addr) -> const sockaddr*
{
    const auto *const sa {reinterpret_cast<const sockaddr*>(addr.data())};
    assert(sa != nullptr);
    return sa;
}

auto Network::get_pointer(SockAddr& addr) -> sockaddr*
{
    auto *const sa {reinterpret_cast<sockaddr*>(addr.data())};
    assert(sa != nullptr);
    return sa;
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
    return get_sockaddr(sa, size);
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

#ifdef HAVE_SOCKADDR_SA_LEN

auto Network::get_sun_length(const sockaddr_un* sun,
                             std::size_t size) -> std::size_t
{
    const auto path_len {get_sun_path_length(sun, size)};
    const auto min_size {sizeof *sun - sizeof sun->sun_path + path_len};
    return std::max(sizeof(sockaddr), min_size);
}

#endif

auto Network::get_sun_path_length(const sockaddr_un* sun,
                                  std::size_t size) -> std::size_t
{
    static constexpr auto path_offset {offsetof(sockaddr_un, sun_path)};

    assert(size >= path_offset);
    assert(size <= path_offset + sizeof sun->sun_path);
    size = std::max(size, path_offset);
    size = std::min(size, path_offset + sizeof sun->sun_path);
#ifdef HAVE_SOCKADDR_SA_LEN
    std::size_t len {sun->sun_len};
    assert(len >= path_offset);
    assert(len <= size);
    len = std::max(len, path_offset);
    len = std::min(len, size);
    const auto max_path_len {std::min(size, len) - path_offset};
#else
    const auto max_path_len {std::min(size - path_offset,
                                      sizeof sun->sun_path)};
#endif
    return strnlen(sun->sun_path, max_path_len);
}

#endif

auto Network::is_valid(const SockAddr& addr, bool verbose) -> bool
{
    if (verbose) {
        std::cerr << "Validating socket address: "
                  << addr
                  << std::endl;
    }

    if (addr.empty()) {
        return false;
    }

    const auto family {get_family(addr)};

    if (verbose) {
        std::cerr << std::left
                  << std::setw(key_width)
                  << "    Family:"
                  << std::right
                  << std::setw(value_width)
                  << family
                  << std::endl;
    }

    switch (family) {
#ifndef _WIN32
    case AF_UNIX:
#endif
    case AF_INET:
    case AF_INET6:
        break;
    default:
        return false;
    }

    const auto max_size {get_max_size(addr)};
    const auto min_size {get_min_size(addr)};

    if (verbose) {
        std::cerr << std::left
                  << std::setw(key_width)
                  << "    Actual size:"
                  << std::right
                  << std::setw(value_width)
                  << addr.size()
                  << std::endl
                  << std::left
                  << std::setw(key_width)
                  << "    Minimum size:"
                  << std::right
                  << std::setw(value_width)
                  << min_size
                  << std::endl
                  << std::left
                  << std::setw(key_width)
                  << "    Maximum size:"
                  << std::right
                  << std::setw(value_width)
                  << max_size
                  << std::endl;
    }

    if (!(min_size <= addr.size() && addr.size() <= max_size)) {
        return false;
    }

#ifdef HAVE_SOCKADDR_SA_LEN

    const auto sa_len {static_cast<std::size_t>(get_sa_length(addr))};

    if (verbose) {
        std::cerr << std::left
                  << std::setw(key_width)
                  << "    Stored length:"
                  << std::right
                  << std::setw(value_width)
                  << sa_len
                  << std::endl;
    }

    if (family == AF_UNIX) {
        if (!(min_size <= sa_len && sa_len <= addr.size())) {
            return false;
        }

#ifndef _WIN32

        const auto *const sun {reinterpret_cast<const sockaddr_un*>(addr.data())};
        const auto sun_len {get_sun_length(sun, addr.size())};

        if (verbose) {
            std::cerr << std::left
                      << std::setw(key_width)
                      << "    Computed length:"
                      << std::right
                      << std::setw(value_width)
                      << sun_len
                      << std::endl;
        }

        if (!(sa_len == sun_len)) {
            return false;
        }

#endif

    }
    else {
        if (!(sa_len == addr.size())) {
            return false;
        }
    }

#endif

    return true;
}
