#include "network-address.h"    // Address, struct sockaddr,
                                // socklen_t, std::ostream,
                                // std::string
#include "network-buffer.h"     // Buffer
#include "stream-address.h"     // operator<<()

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // connect(), htons(), inet_ntop()
#else
#include <arpa/inet.h>  // inet_ntop()
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // connect(), htons()
#endif

#include <cassert>      // assert()
#include <cerrno>       // errno
#include <cstddef>      // offsetof()
#include <cstring>      // std::strerror()
#include <iomanip>      // std::hex, std::setfill(), std::setw()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

Network::Address::Address(const sockaddr* t_addr, socklen_t t_addrlen) :
    value(reinterpret_cast<const char*>(t_addr), t_addrlen)
{
}

bool Network::Address::operator<(const Address& t_address) const
{
    return (family() < t_address.family() ||
            port() < t_address.port() ||
            text() < t_address.text());
}

bool Network::Address::operator>(const Address& t_address) const
{
    return (family() > t_address.family() ||
            port() > t_address.port() ||
            text() > t_address.text());
}

bool Network::Address::operator==(const Address& t_address) const
{
    return (family() == t_address.family() &&
            port() == t_address.port() &&
            text() == t_address.text());
}

Network::Result Network::Address::connect(sock_fd_type t_fd, bool t_verbose) const
{
    assert(!empty());

    if (t_verbose) {
        std::cerr << "Trying "
                  << *this
                  << std::endl;
    }

    std::string error;
    const int code = ::connect(t_fd, addr(), addrlen());

    if (code == connect_error) {
        std::ostringstream oss;
        oss << "connect("
            << t_fd
            << ", "
            << *this
            << ") returned "
            << code
            << ": "
            << std::strerror(errno)
            << " (errno = "
            << errno
            << ')';
        error = oss.str();
    }

    return Result(errno, error);
}

bool Network::Address::empty() const
{
    return value.empty();
}

Network::Address::family_type Network::Address::family() const
{
    switch(sa_family()) {
#ifndef _WIN32
    case AF_UNIX:
        return sun_family();
#endif
    case AF_INET:
        return sin_family();
    case AF_INET6:
        return sin6_family();
    default:
        return sa_family();
    }
}

Network::Address::port_type Network::Address::port() const
{
    switch(sa_family()) {
    case AF_INET:
        return htons(sin_port());
    case AF_INET6:
        return htons(sin6_port());
    default:
        return 0;
    }
}

std::string Network::Address::text() const
{
    switch(sa_family()) {
#ifndef _WIN32
    case AF_UNIX:
        return sun_text();
#endif
    case AF_INET:
        return sin_text();
    case AF_INET6:
        return sin6_text();
    default:
        return sa_text();
    }
}

const sockaddr* Network::Address::addr() const
{
    return reinterpret_cast<const sockaddr*>(value.data());
}

socklen_t Network::Address::addrlen() const
{
    return static_cast<socklen_t>(value.size());
}

Network::Address::operator const sockaddr&() const
{
    return *reinterpret_cast<const sockaddr*>(value.data());
}

std::string Network::Address::sa_data() const
{
    static const std::size_t offset = offsetof(sockaddr, sa_data);
    return std::string(value.data() + offset,
                       value.size() - offset);
}

Network::Address::family_type Network::Address::sa_family() const
{
    const sockaddr& sa = static_cast<const sockaddr&>(*this);
    return sa.sa_family;
}

std::string Network::Address::sa_text() const
{
    const char* data = value.data();
    const std::size_t size = value.size();
    std::ostringstream oss;
    oss << std::hex;

    for(const char* p = data; p < data + size; p++) {
        short ch = static_cast<short>(*p & 0xFF);
        oss << std::setfill('0')
            << std::setw(2)
            << ch;
    }

    const std::string str(oss.str());
    assert(str.size() == size * 2);
    std::string result("0x");

    if (str.empty()) {
        result += "0";
    }
    else {
        result += str;
    }

    return result;
}

Network::Address::operator const sockaddr_in&() const
{
    return *reinterpret_cast<const sockaddr_in*>(value.data());
}

in_addr Network::Address::sin_addr() const
{
    const sockaddr_in& sin = static_cast<const sockaddr_in&>(*this);
    return sin.sin_addr;
}

Network::Address::family_type Network::Address::sin_family() const
{
    const sockaddr_in& sin = static_cast<const sockaddr_in&>(*this);
    return sin.sin_family;
}

Network::Address::port_type Network::Address::sin_port() const
{
    const sockaddr_in& sin = static_cast<const sockaddr_in&>(*this);
    return sin.sin_port;
}

std::string Network::Address::sin_text() const
{
    Buffer buffer(INET_ADDRSTRLEN);
    const sockaddr_in& sin = static_cast<const sockaddr_in&>(*this);
    ::inet_ntop(AF_INET, &sin.sin_addr, &buffer[0], buffer.size());
    return buffer;
}

Network::Address::operator const sockaddr_in6&() const
{
    return *reinterpret_cast<const sockaddr_in6*>(value.data());
}

in6_addr Network::Address::sin6_addr() const
{
    const sockaddr_in6& sin6 = static_cast<const sockaddr_in6&>(*this);
    return sin6.sin6_addr;
}

Network::Address::family_type Network::Address::sin6_family() const
{
    const sockaddr_in6& sin6 = static_cast<const sockaddr_in6&>(*this);
    return sin6.sin6_family;
}

Network::Address::port_type Network::Address::sin6_port() const
{
    const sockaddr_in6& sin6 = static_cast<const sockaddr_in6&>(*this);
    return sin6.sin6_port;
}

std::string Network::Address::sin6_text() const
{
    Buffer buffer(INET6_ADDRSTRLEN);
    const in6_addr& in6 = sin6_addr();
    ::inet_ntop(AF_INET6, &in6, &buffer[0], buffer.size());
    return buffer;
}

#ifndef _WIN32
Network::Address::operator const sockaddr_un&() const
{
    return *reinterpret_cast<const sockaddr_un*>(value.data());
}

Network::Address::family_type Network::Address::sun_family() const
{
    const sockaddr_un& sun = static_cast<const sockaddr_un&>(*this);
    return sun.sun_family;
}

std::string Network::Address::sun_path() const
{
    const sockaddr_un& sun = static_cast<const sockaddr_un&>(*this);
    return std::string(sun.sun_path);
}

std::string Network::Address::sun_text() const
{
    return sun_path();
}
#endif
