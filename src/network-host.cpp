#include "network-host.h"       // Address, Host, Result

#ifdef _WIN32
#include <winsock2.h>   // connect()
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#include <sys/socket.h> // connect()
#endif

#include <cerrno>       // errno
#include <cstring>      // std::strerror()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

Network::Host::Host(const addrinfo& other) :
    address(other.ai_addr, other.ai_addrlen),
    cname(other.ai_canonname == NULL ? "" : other.ai_canonname)
{
}

Network::Host& Network::Host::operator=(const addrinfo& other)
{
    address = Address(other.ai_addr, other.ai_addrlen);
    cname = other.ai_canonname == NULL ? "" : other.ai_canonname;
    return *this;
}

bool Network::Host::operator<(const Host& other) const
{
    return address < other.address;
}

bool Network::Host::operator>(const Host& other) const
{
    return address > other.address;
}

bool Network::Host::operator==(const Host& other) const
{
    return address == other.address;
}

Network::Host::operator Address() const
{
    return address;
}

Network::Hostname Network::Host::canonical_name() const
{
    return cname;
}

Network::Result Network::Host::connect(sock_fd_type fd, bool verbose) const
{
    if (verbose && address.addrlen()) {
        std::cerr << "Trying "
                  << address
                  << std::endl;
    }

    std::string error;
    const int code = ::connect(fd, address.addr(), address.addrlen());

    if (code == Host::connect_error) {
        std::ostringstream os;
        os << "connect("
           << fd
           << ") returned "
           << code
           << ": "
           << std::strerror(errno)
           << " (errno = "
           << errno
           << ')';
        error = os.str();
    }

    return Result(errno, error);
}
