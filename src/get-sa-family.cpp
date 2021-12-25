#include "network/get-sa-family.h"      // SockAddr, family_type,
                                        // get_sa_family(), sockaddr
#include "network/get-pointer.h"        // SockAddr, get_pointer(),
                                        // sockaddr

auto Network::get_sa_family(const SockAddr& addr) -> Network::family_type
{
    const auto *const sa {get_pointer(addr)};
    return sa->sa_family;
}
