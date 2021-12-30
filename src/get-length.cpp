#include "network/get-length.h"         // SockAddr, get_length(),
                                        // sock_len_type
#include "network/get-sa-length.h"      // get_sa_length()

auto Network::get_length(const SockAddr& addr) -> Network::sock_len_type
{
    const auto length {get_sa_length(addr)};
    return length == 0 ? addr.size() : length;
}
