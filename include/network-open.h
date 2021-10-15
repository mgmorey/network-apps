#ifndef NETWORK_OPEN_H
#define NETWORK_OPEN_H

#include "network-endpoint.h"   // Endpoint
#include "network-fd.h"         // Fd
#include "network-hints.h"      // Hints
#include "network-result.h"     // Result
#include "network-socket.h"     // Socket, SocketResult
#include "network-types.h"      // SockAddr, operator<<()

#ifdef _WIN32
#include <winsock2.h>       // sockaddr, socklen_t
#include <ws2tcpip.h>       // socklen_t
#else
#include <netdb.h>          // socklen_t
#include <sys/socket.h>     // sockaddr, socklen_t
#endif

#include <vector>       // std::vector

namespace Network
{
    typedef int (*open_method_type)(fd_type, const sockaddr*, socklen_t);

    class Open
    {
    public:
        explicit Open(open_method_type t_method,
                      std::string t_name,
                      bool t_verbose);
        SocketResult operator()(const Socket& t_socket) const;
        SocketResult open(const Socket& t_socket) const;
        Result open(Fd t_fd, const Socket& t_socket) const;

    private:
        open_method_type m_method {nullptr};
        std::string m_name {""};
        bool m_verbose {false};
    };

    extern Result open(Fd fd,
                       const SockAddr& sock_addr,
                       open_method_type method,
                       std::string name,
                       bool verbose);
    extern std::vector<SocketResult> open(const Endpoint& endpoint,
                                          const Hints* hints,
                                          open_method_type method,
                                          std::string name,
                                          bool verbose);
}

#endif
