#ifndef NETWORK_GET_SA_FAMILY_H
#define NETWORK_GET_SA_FAMILY_H

#include "network/bytes.h"              // Bytes
#include "network/family-type.h"        // family_type

namespace Network
{
    extern auto get_sa_family(const Bytes& addr,
                              family_type family = 0) -> family_type;
}

#endif
