#include "network-address.h"
#include "network-endpoint.h"
#include "network-string.h"

#ifdef _WIN32
#include <winsock2.h>   // connect(),
#include <ws2tcpip.h>   // NI_MAXHOST, NI_MAXSERV, struct addrinfo,
                        // gai_strerror(), getnameinfo(), socklen_t
#else
#include <netdb.h>      // NI_MAXHOST, struct addrinfo, getnameinfo()
#include <sys/socket.h> // connect(), socklen_t
#endif

#include <cassert>      // assert()
#include <cerrno>       // errno
#include <cstring>      // std::strerror()
#include <iostream>     // std::cerr, std::endl

Network::Address::Address()
{
}

Network::Address::Address(const Address& other)
{
    copy(other);
}

Network::Address::Address(const struct addrinfo& other)
{
    copy(other);
}

Network::Address::~Address()
{
}

Network::Address& Network::Address::operator=(const Address& other)
{
    copy(other);
    return *this;
}

Network::Address& Network::Address::operator=(const struct addrinfo& other)
{
    copy(other);
    return *this;
}

bool Network::Address::operator<(const Address& other) const
{
    return (addr < other.addr);
}

bool Network::Address::operator>(const Address& other) const
{
    return (addr > other.addr);
}

bool Network::Address::operator!=(const Address& other) const
{
    return (addr != other.addr);
}

bool Network::Address::operator==(const Address& other) const
{
    return (addr == other.addr);
}

int Network::Address::connect(int fd) const
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
    }

    return error;
}

Network::Endpoint Network::Address::endpoint(int flags) const
{
    assert(size());
    Hostname host(NI_MAXHOST, '\0');
    Service service(NI_MAXSERV, '\0');
    int error = ::getnameinfo(data(), size(),
                              &host[0], host.size(),
                              &service[0], service.size(),
                              flags);

    if (error != 0) {
        std::cerr << "getnameinfo() returned "
                  << error
                  << " ("
                  << ::gai_strerror(error)
                  << ')'
                  << std::endl;
    }

    trim_zeros(host);
    trim_zeros(service);
    return(Endpoint(host, service));
}

void Network::Address::copy(const Address& other)
{
    addr = other.addr;
}

void Network::Address::copy(const struct addrinfo& other)
{
    assert(other.ai_addr != NULL);
    assert(other.ai_addrlen != 0);
    addr.clear();
    const char* data = reinterpret_cast<const char*>(other.ai_addr);
    std::size_t size = other.ai_addrlen;
    addr.append(data, size);
}

const struct sockaddr* Network::Address::data() const
{
    return reinterpret_cast<const struct sockaddr*>(addr.data());
}

socklen_t Network::Address::size() const
{
    return static_cast<socklen_t>(addr.size());
}
