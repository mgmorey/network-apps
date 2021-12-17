#include "network/get-sa-length.h"      // SockAddr, get_sa_length(),
                                        // sockaddr, sock_len_type

#include <cassert>      // assert()

auto Network::get_sa_length(const SockAddr& addr) -> Network::sock_len_type
{
#ifdef HAVE_SOCKADDR_SA_LEN
    const auto *const sa {reinterpret_cast<const sockaddr*>(addr.data())};
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
