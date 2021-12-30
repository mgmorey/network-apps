#include "network/get-sa-pointer.h"     // SockAddr, get_sa_pointer(),
                                        // sockaddr, sockaddr_in,
                                        // sockaddr_in6

auto Network::get_sa_pointer(const SockAddr& addr) -> const sockaddr*
{
    const void* pointer = addr.data();
    return static_cast<const sockaddr*>(pointer);
}

auto Network::get_sa_pointer(SockAddr& addr) -> sockaddr*
{
    void* pointer = addr.data();
    return static_cast<sockaddr*>(pointer);
}

auto Network::get_sa_pointer(const sockaddr_in* sin) -> const sockaddr*
{
    const void* pointer = sin;
    return static_cast<const sockaddr*>(pointer);
}

auto Network::get_sa_pointer(sockaddr_in* sin) -> sockaddr*
{
    void* pointer = sin;
    return static_cast<sockaddr*>(pointer);
}

auto Network::get_sa_pointer(const sockaddr_in6* sin6) -> const sockaddr*
{
    const void* pointer = sin6;
    return static_cast<const sockaddr*>(pointer);
}

auto Network::get_sa_pointer(sockaddr_in6* sin6) -> sockaddr*
{
    void* pointer = sin6;
    return static_cast<sockaddr*>(pointer);
}
