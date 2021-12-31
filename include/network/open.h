#ifndef NETWORK_OPEN_H
#define NETWORK_OPEN_H

#include "network/bytes.h"              // Bytes
#include "network/endpoint.h"           // Endpoint
#include "network/fdresultvector.h"     // Fd, FdResultVector
#include "network/hints.h"              // Hints
#include "network/result.h"             // Result
#include "network/socket.h"             // Socket

#ifdef _WIN32
#include <winsock2.h>       // sockaddr, socklen_t
#include <ws2tcpip.h>       // socklen_t
#else
#include <netdb.h>          // socklen_t
#include <sys/socket.h>     // sockaddr, socklen_t
#endif

#include <string>       // std::string
#include <utility>      // std::pair

namespace Network
{
    using OpenFunction = int (*)(fd_type, const sockaddr*, socklen_t);
    using OpenHandler = std::pair<OpenFunction, const char*>;

    extern auto open(const OpenHandler& handler,
                     const Endpoint& endpoint,
                     const Hints* hints,
                     bool verbose) -> FdResultVector;
    extern auto open(const OpenHandler& handler, Fd fd,
                     const Bytes& addr,
                     bool verbose) -> Result;
}

#endif
