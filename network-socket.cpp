#include "network-socket.h"
#include "network-address.h"
#include "network-addrinfo.h"

#ifdef _WIN32
#include <winsock2.h>		// closesocket(), connect(),
                                // gethostname(), socket()
#include <ws2tcpip.h>		// NI_MAXHOST, struct addrinfo,
                                // getaddrinfo(), getnameinfo(),
                                // socklen_t
#else
#include <netdb.h>		// NI_MAXHOST, struct addrinfo,
                                // getaddrinfo(), getnameinfo()
#include <sys/socket.h>		// AF_UNSPEC, connect(), socket(),
                                // socklen_t
#include <unistd.h>		// close(), gethostname()
#endif

#include <cassert>		// assert()
#include <cerrno>		// errno
#include <cstdlib>		// free()
#include <cstring>		// std::memcpy(), strdup(),
                                // std::strerror()
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
    copy_addrinfo(ai, other);
}

void Socket::init(int protocol, int socktype, int family, int flags)
{
    init_addrinfo(ai, protocol, socktype, family, flags);
}
