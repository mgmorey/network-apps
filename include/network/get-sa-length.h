#ifndef NETWORK_GET_SA_LENGTH_H
#define NETWORK_GET_SA_LENGTH_H

#include "network/bytes.h"              // Bytes
#include "network/sock-len-type.h"      // sock_len_type

namespace Network
{
    extern auto get_sa_length(const Bytes& addr,
                              sock_len_type length = 0) -> sock_len_type;
}

#endif
