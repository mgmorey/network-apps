#include "network-socket.h"
#include "network-address.h"
#include "network-addrinfo.h"

#ifdef _WIN32
#include <winsock2.h>		// struct sockaddr, socket()
#include <ws2tcpip.h>		// struct addrinfo
#else
#include <netdb.h>		// struct addrinfo
#include <sys/socket.h>		// struct sockaddr, socket()
#endif

#include <cstdlib>		// free()
#include <cstring>		// std::memcpy(), strdup()
#include <iostream>		// std::cerr, std::endl

Socket::Socket()
{
    init();
}

Socket::Socket(const Socket& other)
{
    init();
    copy(other);
}

Socket::Socket(const struct addrinfo& other)
{
    init();
    copy(other);
}

Socket::Socket(int protocol, int socktype, int family, int flags)
{
    init(protocol, socktype, family, flags);
}

Socket::Socket(int family, int flags)
{
    init(0, 0, family, flags);
}

Socket::~Socket()
{
    free(ai.ai_addr);
    free(ai.ai_canonname);
}

Socket& Socket::operator=(const Socket& other)
{
    copy(other);
    return *this;
}

Socket& Socket::operator=(const struct addrinfo& other)
{
    copy(other);
    return *this;
}

bool Socket::operator<(const Socket& other) const
{
    return (ai.ai_protocol < other.ai.ai_protocol ||
            ai.ai_socktype < other.ai.ai_socktype ||
            ai.ai_family < other.ai.ai_family ||
            Address(*this) < Address(other));
}

bool Socket::operator>(const Socket& other) const
{
    return (ai.ai_protocol > other.ai.ai_protocol ||
            ai.ai_socktype > other.ai.ai_socktype ||
            ai.ai_family > other.ai.ai_family ||
            Address(*this) > Address(other));
}

bool Socket::operator!=(const Socket& other) const
{
    return (ai.ai_protocol != other.ai.ai_protocol ||
            ai.ai_socktype != other.ai.ai_socktype ||
            ai.ai_family != other.ai.ai_family ||
            Address(*this) != Address(other));
}

bool Socket::operator==(const Socket& other) const
{
    return (ai.ai_protocol == other.ai.ai_protocol &&
            ai.ai_socktype == other.ai.ai_socktype &&
            ai.ai_family == other.ai.ai_family &&
            Address(*this) == Address(other));
}

Socket::operator const struct addrinfo&() const
{
    return ai;
}

Hostname Socket::cname() const
{
    Hostname result;

    if (ai.ai_canonname != NULL) {
        result = ai.ai_canonname;
    }

    return result;
}

int Socket::socket() const
{
    int fd = ::socket(ai.ai_family, ai.ai_socktype, ai.ai_protocol);

    if (fd == -1) {
        std::cerr << "socket("
                  << ai.ai_family
                  << ", "
                  << ai.ai_socktype
                  << ", "
                  << ai.ai_protocol
                  << ") returned "
                  << fd
                  << std::endl;
    }

    return fd;
}

void Socket::copy(const Socket& other)
{
    copy(other.ai);
}

void Socket::copy(const struct addrinfo& other)
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

void Socket::init(int protocol, int socktype, int family, int flags)
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
