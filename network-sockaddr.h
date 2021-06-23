#ifndef NETWORK_SOCKADDR_H
#define NETWORK_SOCKADDR_H

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr
#include <ws2tcpip.h>   // socklen_t
#else
#include <sys/socket.h> // struct sockaddr, socklen_t
#endif

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class SockAddr
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const SockAddr& sockaddr);

    public:
        SockAddr(const struct sockaddr* data, socklen_t size);
        operator std::string() const;

    private:
        bool null;
        std::string value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const SockAddr& sockaddr);
}

#endif
