#include "network/get-sockaddr.h"       // SockAddr, get_sockaddr(),
                                        // sockaddr
#include "network/get-sun-length.h"     // SockAddr, get_sun_length()

#ifndef _WIN32
#include <sys/socket.h> // AF_LOCAL
#include <sys/un.h>     // sockaddr_un
#endif

#include <algorithm>    // std::max(), std::min()
#include <cassert>      // assert()
#include <cstddef>      // std::size_t
#include <cstring>      // std::memcpy(), std::memset()

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
    std::memcpy(addr.sun_path, path, path_size);  // NOLINT
    addr.sun_family = AF_LOCAL;
#ifdef HAVE_SOCKADDR_SA_LEN
    const auto sun_len {std::max(sizeof(sockaddr), min_size)};
    addr.sun_len = sun_len;
    assert(addr.sun_len == get_sun_length(&addr, sizeof addr));  // NOLINT
#endif
    const auto len {std::max(sizeof(sockaddr), min_size + 1)};
    return get_sockaddr(&addr, len);
}

#endif
