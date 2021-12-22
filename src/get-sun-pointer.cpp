#include "network/get-sun-pointer.h"    // SockAddr, get_sun_pointer(),
                                        // sockaddr_un

#ifndef _WIN32

auto Network::get_sun_pointer(const SockAddr& addr) -> const sockaddr_un*
{
    return reinterpret_cast<const sockaddr_un*>(addr.data());
}

auto Network::get_sun_pointer(SockAddr& addr) -> sockaddr_un*
{
    return reinterpret_cast<sockaddr_un*>(addr.data());
}

#endif
