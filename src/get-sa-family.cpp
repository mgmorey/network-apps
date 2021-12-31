#include "network/get-sa-family.h"      // SockAddr, family_type,
                                        // get_sa_family()
#include "network/get-sa-pointer.h"     // get_sa_pointer(), sockaddr

auto Network::get_sa_family(const SockAddr& addr) -> Network::family_type
{
    const auto *const sa {get_sa_pointer(addr)};
    family_type family {};

    if (offsetof(sockaddr, sa_family) + sizeof sa->sa_family <= addr.size()) {
        family = sa->sa_family;
    }

    return family;
}
