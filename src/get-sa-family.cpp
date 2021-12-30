#include "network/get-sa-family.h"      // SockAddr, family_type,
                                        // get_sa_family(), sockaddr
#include "network/get-sa-pointer.h"     // Sockaddr, get_sa_pointer()

auto Network::get_sa_family(const SockAddr& addr) -> Network::family_type
{
    const auto *const sa {get_sa_pointer(addr)};
    return sa->sa_family;
}
