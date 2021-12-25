#include "network/get-sockaddr.h"       // SockAddr, get_sockaddr(),
                                        // sockaddr, sockaddr_in,
                                        // sockaddr_in6, sockaddr_un
#include "network/get-sa-pointer.h"     // get_sa_pointer()
#include "network/get-sun-length.h"     // get_sun_length()
#include "network/get-sun-pointer.h"    // get_sun_pointer()

#ifndef _WIN32
#include <sys/socket.h>     // AF_LOCAL
#endif

#include <algorithm>    // std::max(), std::min()
#include <cassert>      // assert()
#include <cstddef>      // std::size_t
#include <string>       // std::string

#ifndef _WIN32

auto Network::get_sockaddr(const sockaddr_un* sun,
                           std::size_t size) -> Network::SockAddr
{
    const auto *const sa {get_sa_pointer(sun)};
    return get_sockaddr(sa, size != 0 ? size : sizeof(sockaddr_un));
}

auto Network::get_sockaddr(const Pathname& pathname) -> Network::SockAddr
{
    sockaddr_un addr {};
    const auto path {pathname.value_or("")};
    const auto path_size {std::min(path.size(), sizeof addr.sun_path - 1)};
    const auto min_size {sizeof addr - sizeof addr.sun_path + path_size};
    path.copy(addr.sun_path, path_size);  // NOLINT
    addr.sun_family = AF_LOCAL;
#ifdef HAVE_SOCKADDR_SA_LEN
    addr.sun_len = std::max(sizeof(sockaddr), min_size);
    assert(addr.sun_len == get_sun_length(&addr, sizeof addr));  // NOLINT
#endif
    return get_sockaddr(&addr, std::max(sizeof(sockaddr), min_size + 1));
}

#endif
