#include "network/get-pointer.h"        // SockAddr, get_pointer(),
                                        // sockaddr

auto Network::get_pointer(const SockAddr& addr) -> const sockaddr*
{
    return {reinterpret_cast<const sockaddr*>(addr.data())};
}

auto Network::get_pointer(SockAddr& addr) -> sockaddr*
{
    return {reinterpret_cast<sockaddr*>(addr.data())};
}
