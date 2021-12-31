#ifndef NETWORK_GET_LENGTH_H
#define NETWORK_GET_LENGTH_H

#include "network/bytes.h"              // Bytes
#include "network/sock-len-type.h"      // sock_len_type
#include "network/sockaddr.h"           // SockAddr

namespace Network
{
    extern auto get_length(const SockAddr& addr) -> sock_len_type;
}

#endif
