#include "network/get-sun-pointer.h"    // Bytes, get_sun_pointer(),
                                        // sockaddr_un

#ifndef _WIN32

auto Network::get_sun_pointer(const Bytes& addr) -> const sockaddr_un*
{
    const void* pointer = addr.data();
    return static_cast<const sockaddr_un*>(pointer);
}

auto Network::get_sun_pointer(Bytes& addr) -> sockaddr_un*
{
    void* pointer = addr.data();
    return static_cast<sockaddr_un*>(pointer);
}

#endif
