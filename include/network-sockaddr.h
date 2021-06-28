#ifndef NETWORK_SOCKADDR_H
#define NETWORK_SOCKADDR_H

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr, struct sockaddr_in,
                        // sockaddr_in6
#include <ws2tcpip.h>   // socklen_t
#else
#include <netinet/in.h> // struct sockaddr_in, sockaddr_in6
#include <sys/socket.h> // struct sockaddr, socklen_t
#endif

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class SockAddr
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const SockAddr& sa);

    public:
        SockAddr(const sockaddr* addr, socklen_t len);
        bool operator<(const SockAddr& other) const;
        bool operator>(const SockAddr& other) const;
        bool operator==(const SockAddr& other) const;
        operator const sockaddr*() const;
        operator const sockaddr&() const;
        operator const sockaddr_in*() const;
        operator const sockaddr_in&() const;
        operator const sockaddr_in6*() const;
        operator const sockaddr_in6&() const;
        operator std::string() const;
        std::string addr() const;
        unsigned family() const;
        unsigned port() const;
        socklen_t size() const;

    private:
        std::string sa_data() const;
        unsigned sa_family() const;
        unsigned sa_length() const;
        in_addr sin_addr() const;
        unsigned sin_family() const;
        unsigned sin_port() const;
        in6_addr sin6_addr() const;
        unsigned sin6_family() const;
        unsigned sin6_port() const;

        std::string value;
    };

    extern std::string to_hexadecimal(const std::string& value);
    extern std::string to_string(const in_addr& addr);
    extern std::string to_string(const in6_addr& addr);
    extern std::ostream& operator<<(std::ostream& os,
                                    const SockAddr& sa);
    extern std::ostream& operator<<(std::ostream& os,
                                    const in_addr& addr);
}

#endif
