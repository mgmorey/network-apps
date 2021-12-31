#include "network/get-sin6-pointer.h"   // Bytes,
                                        // get_sin6_pointer(),
                                        // sockaddr_in6

auto Network::get_sin6_pointer(const Bytes& addr) -> const sockaddr_in6*
{
    const void* pointer = addr.data();
    return static_cast<const sockaddr_in6*>(pointer);
}

auto Network::get_sin6_pointer(Bytes& addr) -> sockaddr_in6*
{
    void* pointer = addr.data();
    return static_cast<sockaddr_in6*>(pointer);
}
