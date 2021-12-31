#include "network/get-sa-pointer.h"     // Bytes, get_sa_pointer(),
                                        // sockaddr, sockaddr_in,
                                        // sockaddr_in6

auto Network::get_sa_pointer(const Bytes& addr) -> const sockaddr*
{
    const void* pointer = addr.data();
    return static_cast<const sockaddr*>(pointer);
}

auto Network::get_sa_pointer(Bytes& addr) -> sockaddr*
{
    void* pointer = addr.data();
    return static_cast<sockaddr*>(pointer);
}
