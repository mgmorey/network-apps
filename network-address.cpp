#include "network-address.h"    // Address, Result

#ifdef _WIN32
#include <winsock2.h>   // connect()
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#include <sys/socket.h> // connect()
#endif

#include <cassert>      // assert()
#include <cerrno>       // errno
#include <cstring>      // std::strerror()
#include <sstream>      // std::ostringstream

Network::Address::Address(const struct addrinfo& other) :
    addr(other.ai_addr, other.ai_addrlen),
    name(other.ai_canonname == NULL ? "" : other.ai_canonname)
{
}

Network::Address& Network::Address::operator=(const struct addrinfo& other)
{
    addr = SockAddr(other.ai_addr, other.ai_addrlen);
    name = (other.ai_canonname == NULL ? "" : other.ai_canonname);
    return *this;
}

bool Network::Address::operator<(const Address& other) const
{
    return std::string(addr) < std::string(other.addr);
}

bool Network::Address::operator>(const Address& other) const
{
    return std::string(addr) > std::string(other.addr);
}

bool Network::Address::operator==(const Address& other) const
{
    return std::string(addr) == std::string(other.addr);
}

Network::Address::operator SockAddr() const
{
    return addr;
}

Network::Hostname Network::Address::canonical_name() const
{
    return name;
}

Network::Result Network::Address::connect(int fd) const
{
    std::string error;
    int code = ::connect(fd, addr.data(), addr.size());

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
