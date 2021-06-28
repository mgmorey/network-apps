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
                                        const SockAddr& sa);

    public:
        SockAddr(const sockaddr* addr, socklen_t len);
        bool operator<(const SockAddr& other) const;
        bool operator>(const SockAddr& other) const;
        bool operator==(const SockAddr& other) const;
        operator const sockaddr*() const;
        operator const sockaddr&() const;
        operator std::string() const;
        socklen_t size() const;

    private:
        std::string sa_data() const;
        unsigned sa_family() const;
        unsigned sa_length() const;

        std::string value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const SockAddr& sa);
    extern std::string to_hexadecimal(const std::string& value);
}

#endif
