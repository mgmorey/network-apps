#include "network/get-sa-pointer.h"     // SockAddr, get_sa_pointer(),
                                        // sockaddr

auto Network::get_sa_pointer(const SockAddr& addr) -> const sockaddr*
{
    return reinterpret_cast<const sockaddr*>(addr.data());
}

auto Network::get_sa_pointer(SockAddr& addr) -> sockaddr*
{
    return reinterpret_cast<sockaddr*>(addr.data());
}
