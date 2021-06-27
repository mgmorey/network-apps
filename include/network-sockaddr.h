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
        friend class EndpointResult;
        friend class Host;
        friend std::ostream& operator<<(std::ostream& os,
                                        const SockAddr& sa);

    public:
        SockAddr(const sockaddr* addr, socklen_t len);
        bool operator<(const SockAddr& other) const;
        bool operator>(const SockAddr& other) const;
        bool operator==(const SockAddr& other) const;
        std::string data() const;
        unsigned short family() const;
        unsigned short length() const;

    private:
        static unsigned extract_family(const char*& data, std::size_t& size);
        static unsigned extract_length(const char*& data, std::size_t& size);
        static std::string to_hexadecimal(const std::string& value);
        operator const sockaddr*() const;
        operator std::string() const;
        socklen_t size() const;

        std::string value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const SockAddr& sa);
}

#endif
