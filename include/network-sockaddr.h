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
        friend class Address;
        friend class EndpointResult;
        friend std::ostream& operator<<(std::ostream& os,
                                        const SockAddr& addr);

    public:
        SockAddr(const sockaddr* addr, socklen_t len);
        bool operator<(const SockAddr& other) const;
        bool operator>(const SockAddr& other) const;
        bool operator==(const SockAddr& other) const;

    private:
        operator const sockaddr*() const;
        socklen_t length() const;

        std::string value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const SockAddr& addr);
}

#endif
