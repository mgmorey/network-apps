#include "network-address.h"    // Address, Hostname, Result, Service
#include "network-endpoint.h"   // Endpoint, EndpointResult,
                                // get_hostname(), get_service()
#include "network-string.h"     // resize()

#ifdef _WIN32
#include <winsock2.h>   // connect(),
#include <ws2tcpip.h>   // NI_MAXHOST, NI_MAXSERV, struct addrinfo,
                        // gai_strerror(), getnameinfo(), socklen_t
#else
#include <netdb.h>      // NI_MAXHOST, NI_MAXSERV, struct addrinfo,
                        // gai_strerror(), getnameinfo()
#include <sys/socket.h> // connect(), socklen_t
#endif

#include <cassert>      // assert()
#include <cerrno>       // errno
#include <cstring>      // std::strerror()
#include <sstream>      // std::ostringstream

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

Network::Result Network::Address::connect(int fd) const
{
    assert(size());
    std::string error;
    int result = ::connect(fd, data(), size());

    if (result == Address::CONNECT_ERROR) {
        std::ostringstream os;
        os << "connect("
           << fd
           << ") returned "
           << error
           << ": "
           << std::strerror(errno);
        error = os.str();
    }

    return Result(result, error);
}

Network::EndpointResult Network::Address::endpoint(int flags) const
{
    assert(size());
    std::string error;
    Hostname host(NI_MAXHOST, '\0');
    Service service(NI_MAXSERV, '\0');
    int result = ::getnameinfo(data(), size(),
                              &host[0], host.size(),
                              &service[0], service.size(),
                              flags);

    if (result != 0) {
        std::ostringstream os;
        os << "getnameinfo() returned "
           << result
           << " ("
           << ::gai_strerror(result)
           << ')';
        error = os.str();
    }

    Endpoint endpoint(resize(host), resize(service));
    return EndpointResult(endpoint, error);
}

Network::Hostname Network::Address::hostname(int flags) const
{
    return get_hostname(endpoint(flags).first);
}

Network::Service Network::Address::service(int flags) const
{
    return get_service(endpoint(flags).first);
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
