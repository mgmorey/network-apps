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
