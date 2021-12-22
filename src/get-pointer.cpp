#include "network/get-pointer.h"        // SockAddr, get_pointer(),
                                        // sockaddr
#include "network/get-sa-pointer.h"     // SockAddr, get_sa_pointer(),
                                        // sockaddr

auto Network::get_pointer(const SockAddr& addr) -> const sockaddr*
{
    return {get_sa_pointer(addr)};
}

auto Network::get_pointer(SockAddr& addr) -> sockaddr*
{
    return {get_sa_pointer(addr)};
}
