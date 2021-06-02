#include "network.h"

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

Address::Address()
{
}

Address::Address(const Address& other)
{
    copy(other);
}

Address::Address(const struct addrinfo& other)
{
    copy(other);
}

Address::~Address()
{
}

Address& Address::operator=(const Address& other)
{
    copy(other);
    return *this;
}

Address& Address::operator=(const struct addrinfo& other)
{
    copy(other);
    return *this;
}

bool Address::operator<(const Address& other) const
{
    return (addr < other.addr);
}

bool Address::operator>(const Address& other) const
{
    return (addr > other.addr);
}

bool Address::operator!=(const Address& other) const
{
    return (addr != other.addr);
}

bool Address::operator==(const Address& other) const
{
    return (addr == other.addr);
}

int Address::connect(int fd) const
{
    assert(size());
    int error = ::connect(fd, data(), size());

    if (error == -1) {
        std::cerr << "connect("
                  << fd
                  << ") returned "
                  << error
                  << ": "
                  << std::strerror(errno)
                  << std::endl;
        goto clean_up_socket;
    }

    return error;

clean_up_socket:
    close_socket(fd);
    return error;
}

Endpoint Address::endpoint(int flags) const
{
    assert(size());
    std::string host(NI_MAXHOST, '\0');
    std::string service(NI_MAXHOST, '\0');
    int error = getnameinfo(data(), size(),
                            &host[0], NI_MAXHOST,
                            &service[0], NI_MAXHOST,
                            flags);

    if (error != 0) {
        std::cerr << "getnameinfo() returned "
                  << error
                  << " ("
                  << gai_strerror(error)
                  << ')'
                  << std::endl;
    }

    trim_zeros(host);
    trim_zeros(service);
    return(Endpoint(host, service));
}

std::string Address::service() const
{
    assert(size());
    return endpoint().second;
}

std::string Address::to_hostname() const
{
    assert(size());
    return endpoint().first;
}

std::string Address::to_string() const
{
    assert(size());
    return endpoint(NI_NUMERICHOST).first;
}

void Address::copy(const Address& other)
{
    addr = other.addr;
}

void Address::copy(const struct addrinfo& other)
{
    assert(other.ai_addr != NULL);
    assert(other.ai_addrlen != 0);
    addr.clear();
    const char* data = reinterpret_cast<const char*>(other.ai_addr);
    std::size_t size = other.ai_addrlen;
    addr.append(data, size);
}

const struct sockaddr* Address::data() const
{
    return reinterpret_cast<const struct sockaddr*>(addr.data());
}

socklen_t Address::size() const
{
    return static_cast<socklen_t>(addr.size());
}
