#ifndef NETWORK_NAME_H
#define NETWORK_NAME_H

#include "network-fd.h"         // Fd
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
    using name_method_type = int(fd_type, sockaddr*, socklen_t *);

    extern std::pair<SockAddr, Result> get_name(const Fd& fd,
                                                const name_method_type* method,
                                                const std::string& name,
                                                bool verbose);
}

#endif
