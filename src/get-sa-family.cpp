#include "network/get-sa-family.h"      // Bytes, family_type,
                                        // get_sa_family()
#include "network/get-sa-pointer.h"     // get_sa_pointer(), sockaddr

auto Network::get_sa_family(const Bytes& addr,
                            family_type family) -> Network::family_type
{
    const auto *const sa {get_sa_pointer(addr)};

    if (offsetof(sockaddr, sa_family) + sizeof sa->sa_family <= addr.size()) {
        if (sa->sa_family != 0) {
            family = sa->sa_family;
        }
    }

    return family;
}
