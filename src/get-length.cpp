#include "network/get-length.h"         // SockAddr, get_length(),
                                        // sock_len_type
#include "network/get-sa-length.h"      // SockAddr, get_sa_length(),
                                        // sock_len_type

auto Network::get_length(const SockAddr& addr) -> Network::sock_len_type
{
    const auto sa_len {get_sa_length(addr)};
    return static_cast<sock_len_type>(sa_len != 0 ? sa_len : addr.size());
}
