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
    addr(other.ai_addr, other.ai_addrlen),
    name(other.ai_canonname == NULL ? "" : other.ai_canonname)
{
}

Network::Host& Network::Host::operator=(const addrinfo& other)
{
    addr = Address(other.ai_addr, other.ai_addrlen);
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

Network::Result Network::Host::connect(sock_fd_type fd, bool verbose) const
{
    if (verbose && addr.size()) {
        std::cerr << "Trying "
                  << addr
                  << std::endl;
    }

    std::string error;
    int code = ::connect(fd, addr, addr.size());

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

const Network::Address& Network::Host::address() const
{
    return addr;
}
