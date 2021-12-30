#include "network/get-sa-length.h"      // SockAddr, get_sa_length(),
                                        // sockaddr, sock_len_type
#include "network/get-sa-pointer.h"     // get_sa_pointer()

auto Network::get_sa_length(const SockAddr& addr) -> Network::sock_len_type
{
#ifdef HAVE_SOCKADDR_SA_LEN
    const auto *const sa {get_sa_pointer(addr)};

    if (addr.empty()) {
        return 0;
    }

    return sa->sa_len;
#else
    static_cast<void>(addr);
    return 0;
#endif
}
