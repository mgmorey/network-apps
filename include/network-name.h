#ifndef NETWORK_NAME_H
#define NETWORK_NAME_H

#include "network-fd.h"         // Fd, fd_type
#include "network-result.h"     // Result
#include "network-types.h"      // SockAddr

#ifdef _WIN32
#include <winsock2.h>       // sockaddr, socklen_t
#include <ws2tcpip.h>       // socklen_t
#else
#include <netdb.h>          // socklen_t
#include <sys/socket.h>     // sockaddr, socklen_t
#endif

#include <utility>      // std::pair

namespace Network
{
    using GetNameFunction = int (*)(fd_type, sockaddr*, socklen_t *);
    using GetNameMethod = std::pair<GetNameFunction, std::string>;
    using SockAddrResult = std::pair<SockAddr, Result>;

    extern SockAddrResult get_name(const GetNameMethod& method,
                                   const Fd& fd,
                                   bool verbose);
}

#endif
