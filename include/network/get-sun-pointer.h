#ifndef NETWORK_GET_SUN_POINTER_H
#define NETWORK_GET_SUN_POINTER_H

#include "network/sockaddr.h"           // SockAddr

#ifndef _WIN32
#include <sys/un.h>         // sockaddr_un
#endif

namespace Network
{
#ifndef _WIN32
    extern auto get_sun_pointer(const SockAddr& addr) -> const sockaddr_un*;
    extern auto get_sun_pointer(SockAddr& addr) -> sockaddr_un*;
#endif
}

#endif
