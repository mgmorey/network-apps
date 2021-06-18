#include "network-address.h"    // Address, Hostname, Result, Service
#include "network-buffer.h"     // Buffer
#include "network-endpoint.h"   // Endpoint, EndpointResult,
                                // get_hostname(), get_service()

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

Network::Address::Address(const Address& other) :
    addr(other.addr)
{
}

Network::Address::Address(const struct addrinfo& other) :
    addr(address(other))
{
}

Network::Address::~Address()
{
}

Network::Address& Network::Address::operator=(const Address& other)
{
    addr = other.addr;
    return *this;
}

Network::Address& Network::Address::operator=(const struct addrinfo& other)
{
    addr = address(other);
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

bool Network::Address::operator==(const Address& other) const
{
    return (addr == other.addr);
}

Network::Result Network::Address::connect(int fd) const
{
    assert(size());
    std::string error;
    int code = ::connect(fd, data(), size());

    if (code == Address::connect_error) {
        std::ostringstream os;
        os << "connect("
           << fd
           << ") returned "
           << code
           << ": "
           << std::strerror(errno);
        error = os.str();
    }

    return Result(code, error);
}

Network::EndpointResult Network::Address::endpoint(int flags) const
{
    assert(size());
    std::string error;
    Buffer host(NI_MAXHOST);
    Buffer service(NI_MAXSERV);
    int code = ::getnameinfo(data(), size(),
                             &host[0], host.size(),
                             &service[0], service.size(),
                             flags);

    if (code != 0) {
        std::ostringstream os;
        os << "getnameinfo() returned "
           << code
           << " ("
           << ::gai_strerror(code)
           << ')';
        error = os.str();
    }

    return EndpointResult(Endpoint(host.data(), service.data()),
                          Result(code, error));
}

Network::Hostname Network::Address::hostname(int flags) const
{
    return get_hostname(endpoint(flags).first);
}

Network::Service Network::Address::service(int flags) const
{
    return get_service(endpoint(flags).first);
}

std::string Network::Address::address(const struct addrinfo& other)
{
    std::string result;
    assert(other.ai_addr != NULL);
    assert(other.ai_addrlen != 0);
    const char* data = reinterpret_cast<const char*>(other.ai_addr);
    std::string::size_type size = other.ai_addrlen;
    result.append(data, size);
    return result;
}

const struct sockaddr* Network::Address::data() const
{
    return reinterpret_cast<const struct sockaddr*>(addr.data());
}

socklen_t Network::Address::size() const
{
    return static_cast<socklen_t>(addr.size());
}
