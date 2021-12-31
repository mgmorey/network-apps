#ifndef NETWORK_GET_NAME_H
#define NETWORK_GET_NAME_H

#include "network/bytes.h"              // Bytes
#include "network/fd.h"                 // Fd, fd_type
#include "network/result.h"             // Result
#include "network/sockaddr.h"           // SockAddr

#ifdef _WIN32
#include <winsock2.h>       // sockaddr, socklen_t
#include <ws2tcpip.h>       // socklen_t
#else
#include <netdb.h>          // socklen_t
#include <sys/socket.h>     // sockaddr, socklen_t
#endif

#include <string>       // std::string
#include <utility>      // std::pair
#include <variant>      // std::variant

namespace Network
{
    using GetNameFunction = int (*)(fd_type, sockaddr*, socklen_t*);
    using GetNameHandler = std::pair<GetNameFunction, const char*>;

    using SockAddrResult = std::variant<SockAddr, Result>;

    extern auto get_name(const GetNameHandler& handler, Fd fd,
                         bool verbose) -> SockAddrResult;
}

#endif
