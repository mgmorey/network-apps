#include "network/is-valid.h"           // SockAddr, is_valid()
#include "network/get-sa-family.h"      // get_sa_family()
#include "network/get-sa-length.h"      // get_sa_length()
#include "network/get-sun-length.h"     // get_sun_length()
#include "network/get-sun-pointer.h"    // get_sun_pointer()
#include "network/sizes.h"              // sockaddr_size_max

#ifdef _WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_LOCAL, AF_UNIX,
                            // sockaddr, sockaddr_in, sockaddr_storage
#include <ws2tcpip.h>       // sockaddr_in6
#else
#include <netinet/in.h>     // sockaddr_in, sockaddr_in6
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_LOCAL, AF_UNIX,
                            // sockaddr, sockaddr_storage
#include <sys/un.h>         // sockaddr_un
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

static auto get_max_size(const Network::SockAddr& addr) -> std::size_t
{
    const auto family {Network::get_sa_family(addr)};

    switch (family) {
    case AF_UNSPEC:
        return Network::sockaddr_size_max;
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
    const auto family {Network::get_sa_family(addr)};

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
        return 0;
    }
}

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

    const auto family {get_sa_family(addr)};

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
    case AF_UNSPEC:
#ifndef _WIN32
    case AF_UNIX:
#endif
    case AF_INET:
    case AF_INET6:
        break;
    default:
        return false;
    }

    const auto max_addr_size {::get_max_size(addr)};
    const auto min_addr_size {::get_min_size(addr)};
    const auto addr_size {addr.size()};

    if (verbose) {
        std::cerr << std::left
                  << std::setw(key_width)
                  << "    Actual size:"
                  << std::right
                  << std::setw(value_width)
                  << addr_size
                  << std::endl
                  << std::left
                  << std::setw(key_width)
                  << "    Minimum size:"
                  << std::right
                  << std::setw(value_width)
                  << min_addr_size
                  << std::endl
                  << std::left
                  << std::setw(key_width)
                  << "    Maximum size:"
                  << std::right
                  << std::setw(value_width)
                  << max_addr_size
                  << std::endl;
    }

    if (!(min_addr_size <= addr_size && addr_size <= max_addr_size)) {
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
        if (!(min_addr_size <= sa_len && sa_len <= addr_size)) {
            return false;
        }

#ifndef _WIN32

        const auto *const sun {get_sun_pointer(addr)};
        const auto sun_len {get_sun_length(sun, addr_size)};

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
    else if (family == AF_INET || family == AF_INET6) {
        if (!(sa_len == addr_size)) {
            return false;
        }
    }

#endif

    return true;
}
