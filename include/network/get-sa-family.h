#ifndef NETWORK_GET_SA_FAMILY_H
#define NETWORK_GET_SA_FAMILY_H

#include "network/family-type.h"        // family_type
#include "network/sockaddr.h"           // SockAddr

namespace Network
{
    extern auto get_sa_family(const SockAddr& addr,
                              family_type family = 0) -> family_type;
}

#endif
