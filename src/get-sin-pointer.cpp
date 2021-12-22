#include "network/get-sin-pointer.h"    // SockAddr,
                                        // get_sin_pointer(),
                                        // get_sin6_pointer(),
                                        // sockaddr_in, sockaddr_in6

auto Network::get_sin_pointer(const SockAddr& addr) -> const sockaddr_in*
{
    return reinterpret_cast<const sockaddr_in*>(addr.data());
}

auto Network::get_sin_pointer(SockAddr& addr) -> sockaddr_in*
{
    return reinterpret_cast<sockaddr_in*>(addr.data());
}

auto Network::get_sin6_pointer(const SockAddr& addr) -> const sockaddr_in6*
{
    return reinterpret_cast<const sockaddr_in6*>(addr.data());
}

auto Network::get_sin6_pointer(SockAddr& addr) -> sockaddr_in6*
{
    return reinterpret_cast<sockaddr_in6*>(addr.data());
}
