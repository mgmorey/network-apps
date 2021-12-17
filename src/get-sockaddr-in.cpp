#include "network/get-sockaddr.h"       // SockAddr, get_sockaddr(),
                                        // sockaddr, sockaddr_in,
                                        // sockaddr_in6

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
