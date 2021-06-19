#include "network-address.h"    // Address, Hostname, Result, Service
#include "network-string.h"     // to_hex()

#ifdef _WIN32
#include <winsock2.h>   // connect()
#include <ws2tcpip.h>   // struct addrinfo, struct sockaddr, socklen_t
#else
#include <netdb.h>      // struct addrinfo, struct sockaddr
#include <sys/socket.h> // connect(), socklen_t
#endif

#include <cassert>      // assert()
#include <cerrno>       // errno
#include <cstring>      // std::strerror()
#include <iostream>     // std::cerr, std::endl
#include <ostream>      // std::ostream
#include <sstream>      // std::ostringstream

Network::Address::Address(const struct addrinfo& other) :
    addr(address(other))
{
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

Network::Result Network::connect(int fd,
                                 const struct addrinfo& ai,
                                 bool is_verbose)
{
    Network::Address address(ai);

    if (is_verbose) {
        std::cerr << "Trying "
                  << address
                  << std::endl;
    }

    return address.connect(fd);
}

std::ostream& Network::operator<<(std::ostream& os, const Address& addr)
{
    os << "Address(addr="
       << to_hex(addr.addr)
       << ')';
    return os;
}
