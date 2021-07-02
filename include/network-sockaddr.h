#ifndef NETWORK_SOCKADDR_H
#define NETWORK_SOCKADDR_H

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr, struct sockaddr_in,
                        // struct sockaddr_in6
#include <ws2tcpip.h>   // socklen_t
#else
#include <netinet/in.h> // struct sockaddr_in, struct sockaddr_in6
#include <sys/socket.h> // struct sockaddr, socklen_t
#include <sys/un.h>     // struct sockaddr_un
#endif

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
#ifdef _WIN32
    typedef unsigned short family_type;
    typedef unsigned short port_type;
#else
    typedef sa_family_t family_type;
    typedef in_port_t port_type;
#endif

    class SockAddr
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const SockAddr& sa);
        friend std::ostream& operator<<(std::ostream& os,
                                        const sockaddr_in& sin);
        friend std::ostream& operator<<(std::ostream& os,
                                        const in_addr& addr);
        friend std::ostream& operator<<(std::ostream& os,
                                        const sockaddr_in6& sin6);
        friend std::ostream& operator<<(std::ostream& os,
                                        const in6_addr& addr);
#ifndef _WIN32
        friend std::ostream& operator<<(std::ostream& os,
                                        const sockaddr_un& sin);
#endif

    public:
        SockAddr(const sockaddr* addr, socklen_t addrlen);
        bool operator<(const SockAddr& other) const;
        bool operator>(const SockAddr& other) const;
        bool operator==(const SockAddr& other) const;
        operator const sockaddr*() const;
        operator const sockaddr&() const;
        operator const sockaddr_in&() const;
        operator const sockaddr_in6&() const;
#ifndef _WIN32
        operator const sockaddr_un&() const;
#endif
        std::string addr() const;
        family_type family() const;
        port_type port() const;
        socklen_t size() const;

    private:
        static std::string to_string(const std::string& value);
        static std::string to_string(const in_addr& addr);
        static std::string to_string(const in6_addr& addr);
        std::string sa_data() const;
        family_type sa_family() const;
        socklen_t sa_length() const;
        in_addr sin_addr() const;
        family_type sin_family() const;
        port_type sin_port() const;
        in6_addr sin6_addr() const;
        family_type sin6_family() const;
        port_type sin6_port() const;
#ifndef _WIN32
        family_type sun_family() const;
        std::string sun_path() const;
#endif

        std::string value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const SockAddr& sa);
    extern std::ostream& operator<<(std::ostream& os,
                                    const sockaddr_in& sin);
    extern std::ostream& operator<<(std::ostream& os,
                                    const in_addr& addr);
    extern std::ostream& operator<<(std::ostream& os,
                                    const sockaddr_in6& sin6);
    extern std::ostream& operator<<(std::ostream& os,
                                    const in6_addr& addr);
#ifndef _WIN32
    extern std::ostream& operator<<(std::ostream& os,
                                    const sockaddr_un& sun);
#endif
}

#endif
