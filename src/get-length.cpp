#include "network/get-length.h"         // SockAddr, get_length(),
                                        // sockaddr, sock_len_type

#include <cassert>      // assert()
#include <cstddef>      // std::size_t

static auto get_sa_length(const Network::SockAddr& addr) -> std::size_t
{
#ifdef HAVE_SOCKADDR_SA_LEN
    const auto *const sa {reinterpret_cast<const sockaddr*>(addr.data())};
    assert(sa != nullptr);

    if (addr.empty()) {
        return 0;
    }

    return sa->sa_len;
#else
    static_cast<void>(addr);
    return 0;
#endif
}

auto Network::get_length(const SockAddr& addr) -> Network::sock_len_type
{
    const auto length {static_cast<std::size_t>(get_sa_length(addr))};
    return static_cast<sock_len_type>(length != 0 ? length : addr.size());
}
