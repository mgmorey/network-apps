#ifndef NETWORK_SOCKADDR_H
#define NETWORK_SOCKADDR_H

#ifdef _WIN32
#include <winsock2.h>   // sockaddr
#else
#include <sys/socket.h> // sockaddr
#include <sys/un.h>     // sockaddr_un
#endif

#include "network-socklen.h"    // socklen_type
#include "network-types.h"      // SockAddr

namespace Network
{
    extern socklen_type get_length(const SockAddr& addr);
    extern socklen_type get_length(SockAddr& addr);
    extern const sockaddr* get_pointer(const SockAddr& addr);
    extern sockaddr* get_pointer(SockAddr& addr);
    extern SockAddr get_sockaddr(const sockaddr* addr_ptr = nullptr,
                                 socklen_type addr_len = 0);
#ifndef _WIN32
    extern SockAddr get_sockaddr(const sockaddr_un* addr_ptr,
                                 socklen_type addr_len);
#endif
}

#endif
