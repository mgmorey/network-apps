#include "network/get-sa-length.h"      // SockAddr, get_sa_length(),
                                        // sockaddr, sock_len_type
#include "network/get-pointer.h"        // SockAddr, get_pointer(),
                                        // sockaddr

#include <cassert>      // assert()

auto Network::get_sa_length(const SockAddr& addr) -> Network::sock_len_type
{
#ifdef HAVE_SOCKADDR_SA_LEN
    const auto *const sa {get_pointer(addr)};
    assert(sa != nullptr);  // NOLINT

    if (addr.empty()) {
        return 0;
    }

    return sa->sa_len;
#else
    static_cast<void>(addr);
    return 0;
#endif
}
