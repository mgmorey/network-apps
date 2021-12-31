#ifndef NETWORK_GET_SUN_POINTER_H
#define NETWORK_GET_SUN_POINTER_H

#include "network/bytes.h"              // Bytes

#ifndef _WIN32
#include <sys/un.h>         // sockaddr_un
#endif

namespace Network
{
#ifndef _WIN32
    extern auto get_sun_pointer(const Bytes& addr) -> const sockaddr_un*;
    extern auto get_sun_pointer(Bytes& addr) -> sockaddr_un*;
#endif
}

#endif
