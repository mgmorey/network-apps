#include "network/get-sa-length.h"      // SockAddr, get_sa_length(),
                                        // sock_len_type
#include "network/get-sa-pointer.h"     // get_sa_pointer(), sockaddr

auto Network::get_sa_length(const SockAddr& addr,
                            sock_len_type length) -> Network::sock_len_type
{
#ifdef HAVE_SOCKADDR_SA_LEN
    const auto *const sa {get_sa_pointer(addr)};

    if (offsetof(sockaddr, sa_len) + sizeof sa->sa_len <= addr.size()) {
        if (sa->sa_len != 0) {
            length = sa->sa_len;
        }
    }
#else
    static_cast<void>(addr);
#endif
    return length;
}
