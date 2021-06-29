#ifndef NETWORK_SOCKADDR_H
#define NETWORK_SOCKADDR_H

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr, struct sockaddr_in,
                        // struct sockaddr_in6
#include <ws2tcpip.h>   // socklen_t
#else
#include <netinet/in.h> // struct sockaddr_in, struct sockaddr_in6
#include <sys/socket.h> // struct sockaddr, socklen_t
#endif

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
#ifdef _WIN32
    typedef u_short family_type;
    typedef u_short port_type;
#else
    typedef sa_family_t family_type;
    typedef in_port_t port_type;
#endif

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
        operator const sockaddr_in&() const;
        operator const sockaddr_in6&() const;
        operator std::string() const;
        std::string addr() const;
        family_type family() const;
        port_type port() const;
        socklen_t size() const;

    private:
        std::string sa_data() const;
        family_type sa_family() const;
        socklen_t sa_length() const;
        in_addr sin_addr() const;
        family_type sin_family() const;
        port_type sin_port() const;
        in6_addr sin6_addr() const;
        family_type sin6_family() const;
        port_type sin6_port() const;

        std::string value;
    };

    extern std::string to_string(const in_addr& addr);
    extern std::string to_string(const in6_addr& addr);
    extern std::string to_string_hex(const std::string& value);
    extern std::ostream& operator<<(std::ostream& os,
                                    const SockAddr& sa);
    extern std::ostream& operator<<(std::ostream& os,
                                    const sockaddr_in& sin);
    extern std::ostream& operator<<(std::ostream& os,
                                    const sockaddr_in6& sin);
    extern std::ostream& operator<<(std::ostream& os,
                                    const in_addr& addr);
    extern std::ostream& operator<<(std::ostream& os,
                                    const in6_addr& addr);
}

#endif
