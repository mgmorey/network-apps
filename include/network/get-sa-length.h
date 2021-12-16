#ifndef NETWORK_GET_SA_LENGTH_H
#define NETWORK_GET_SA_LENGTH_H

#include "network/sock-len-type.h"      // sock_len_type
#include "network/sockaddr.h"           // SockAddr

namespace Network
{
    extern auto get_sa_length(const SockAddr& addr) -> sock_len_type;
}

#endif
