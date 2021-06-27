#include "network-host.h"       // Host, Result, SockAddr

#ifdef _WIN32
#include <winsock2.h>   // connect()
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#include <sys/socket.h> // connect()
#endif

#include <cerrno>       // errno
#include <cstring>      // std::strerror()
#include <sstream>      // std::ostringstream

Network::Host::Host(const addrinfo& other) :
    addr(other.ai_addr, other.ai_addrlen),
    name(other.ai_canonname == NULL ? "" : other.ai_canonname)
{
}

Network::Host& Network::Host::operator=(const addrinfo& other)
{
    addr = SockAddr(other.ai_addr, other.ai_addrlen);
    name = other.ai_canonname == NULL ? "" : other.ai_canonname;
    return *this;
}

bool Network::Host::operator<(const Host& other) const
{
    return addr < other.addr;
}

bool Network::Host::operator>(const Host& other) const
{
    return addr > other.addr;
}

bool Network::Host::operator==(const Host& other) const
{
    return addr == other.addr;
}

Network::Hostname Network::Host::canonical_name() const
{
    return name;
}

Network::Result Network::Host::connect(int fd) const
{
    std::string error;
    int code = ::connect(fd, addr, addr.length());

    if (code == Host::connect_error) {
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

const Network::SockAddr& Network::Host::address() const
{
    return addr;
}
