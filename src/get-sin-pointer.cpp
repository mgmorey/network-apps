#include "network/get-sin-pointer.h"    // Bytes,
                                        // get_sin_pointer(),
                                        // sockaddr_in

auto Network::get_sin_pointer(const Bytes& addr) -> const sockaddr_in*
{
    const void* pointer = addr.data();
    return static_cast<const sockaddr_in*>(pointer);
}

auto Network::get_sin_pointer(Bytes& addr) -> sockaddr_in*
{
    void* pointer = addr.data();
    return static_cast<sockaddr_in*>(pointer);
}
