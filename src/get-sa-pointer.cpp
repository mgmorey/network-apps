#include "network/get-sa-pointer.h"     // SockAddr, get_sa_pointer(),
                                        // sockaddr, sockaddr_in,
                                        // sockaddr_in6

auto Network::get_sa_pointer(const SockAddr& addr) -> const sockaddr*
{
    return reinterpret_cast<const sockaddr*>(addr.data());
}

auto Network::get_sa_pointer(SockAddr& addr) -> sockaddr*
{
    return reinterpret_cast<sockaddr*>(addr.data());
}

auto Network::get_sa_pointer(const sockaddr_in* sin) -> const sockaddr*
{
    return reinterpret_cast<const sockaddr*>(sin);
}

auto Network::get_sa_pointer(sockaddr_in* sin) -> sockaddr*
{
    return reinterpret_cast<sockaddr*>(sin);
}

auto Network::get_sa_pointer(const sockaddr_in6* sin6) -> const sockaddr*
{
    return reinterpret_cast<const sockaddr*>(sin6);
}

auto Network::get_sa_pointer(sockaddr_in6* sin6) -> sockaddr*
{
    return reinterpret_cast<sockaddr*>(sin6);
}

#ifndef _WIN32

auto Network::get_sa_pointer(const sockaddr_un* sun) -> const sockaddr*
{
    return reinterpret_cast<const sockaddr*>(sun);
}

auto Network::get_sa_pointer(sockaddr_un* sun) -> sockaddr*
{
    return reinterpret_cast<sockaddr*>(sun);
}

#endif
