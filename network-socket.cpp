#include "network-socket.h"     // Hostname, Result, Socket
#include "network-address.h"    // Address

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr, socket()
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#include <sys/socket.h> // struct sockaddr, socket()
#endif

#include <cstdlib>      // free()
#include <cstring>      // std::memcpy(), strdup()
#include <sstream>      // std::ostringstream

Network::Socket::Socket()
{
    init();
}

Network::Socket::Socket(const Socket& other)
{
    init();
    copy(other);
}

Network::Socket::Socket(const struct addrinfo& other)
{
    init();
    copy(other);
}

Network::Socket::Socket(int protocol, int socktype, int family, int flags)
{
    init(protocol, socktype, family, flags);
}

Network::Socket::Socket(int family, int flags)
{
    init(0, 0, family, flags);
}

Network::Socket::~Socket()
{
    free(ai.ai_addr);
    free(ai.ai_canonname);
}

Network::Socket& Network::Socket::operator=(const Socket& other)
{
    copy(other);
    return *this;
}

Network::Socket& Network::Socket::operator=(const struct addrinfo& other)
{
    copy(other);
    return *this;
}

bool Network::Socket::operator<(const Socket& other) const
{
    return (ai.ai_protocol < other.ai.ai_protocol ||
            ai.ai_socktype < other.ai.ai_socktype ||
            ai.ai_family < other.ai.ai_family ||
            Address(*this) < Address(other));
}

bool Network::Socket::operator>(const Socket& other) const
{
    return (ai.ai_protocol > other.ai.ai_protocol ||
            ai.ai_socktype > other.ai.ai_socktype ||
            ai.ai_family > other.ai.ai_family ||
            Address(*this) > Address(other));
}

bool Network::Socket::operator==(const Socket& other) const
{
    return (ai.ai_protocol == other.ai.ai_protocol &&
            ai.ai_socktype == other.ai.ai_socktype &&
            ai.ai_family == other.ai.ai_family &&
            Address(*this) == Address(other));
}

Network::Socket::operator const struct addrinfo&() const
{
    return ai;
}

Network::Hostname Network::Socket::cname() const
{
    Hostname result;

    if (ai.ai_canonname != NULL) {
        result = ai.ai_canonname;
    }

    return result;
}

Network::Result Network::Socket::socket() const
{
    std::string error;
    int fd = ::socket(ai.ai_family, ai.ai_socktype, ai.ai_protocol);

    if (fd == SOCKET_BAD) {
        std::ostringstream os;
        os << "socket("
           << ai.ai_family
           << ", "
           << ai.ai_socktype
           << ", "
           << ai.ai_protocol
           << ") returned "
           << fd;
        error = os.str();
    }

    return Result(fd, error);
}

void Network::Socket::copy(const Socket& other)
{
    copy(other.ai);
}

void Network::Socket::copy(const struct addrinfo& other)
{
    ai.ai_flags = other.ai_flags;
    ai.ai_family = other.ai_family;
    ai.ai_socktype = other.ai_socktype;
    ai.ai_protocol = other.ai_protocol;
    ai.ai_addrlen = other.ai_addrlen;
    free(ai.ai_canonname);
    ai.ai_canonname = NULL;

    if (other.ai_canonname != NULL) {
        ai.ai_canonname = strdup(other.ai_canonname);
    }

    free(ai.ai_addr);
    ai.ai_addr = NULL;

    if (other.ai_addr != NULL) {
        ai.ai_addr = static_cast<struct sockaddr*>
            (malloc(ai.ai_addrlen));
        std::memcpy(ai.ai_addr, other.ai_addr, ai.ai_addrlen);
    }

    ai.ai_next = NULL;
}

void Network::Socket::init(int protocol, int socktype, int family, int flags)
{
    std::memset(&ai, '\0', sizeof ai);
    ai.ai_flags = flags;
    ai.ai_family = family;
    ai.ai_socktype = socktype;
    ai.ai_protocol = protocol;
    ai.ai_addrlen = 0;
    ai.ai_canonname = NULL;
    ai.ai_addr = NULL;
    ai.ai_next = NULL;
}
