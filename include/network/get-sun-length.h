#ifndef NETWORK_GET_SUN_LENGTH_H
#define NETWORK_GET_SUN_LENGTH_H

#include "network/sockaddr.h"           // SockAddr

#ifdef _WIN32
#include <winsock2.h>       // sockaddr, sockaddr_in
#else
#include <sys/socket.h>     // sockaddr
#include <sys/un.h>         // sockaddr_un
#endif

namespace Network
{
#ifndef _WIN32
#ifdef HAVE_SOCKADDR_SA_LEN
    extern auto get_sun_length(const sockaddr_un* sun,
                               std::size_t size) -> std::size_t;
#endif
    extern auto get_sun_path_length(const sockaddr_un* sun,
                                    std::size_t size) -> std::size_t;
#endif
}

#endif
