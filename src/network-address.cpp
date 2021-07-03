#include "network-address.h"    // Address, struct sockaddr,
                                // socklen_t, std::ostream,
                                // std::string
#include "network-buffer.h"     // Buffer

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // PF_INET, PF_INET6, PF_UNIX, PF_UNSPEC
                        // inet_ntop()
#else
#include <arpa/inet.h>  // inet_ntop()
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // PF_INET, PF_INET6, PF_UNIX, PF_UNSPEC
#endif

#include <cassert>      // assert()
#include <cstddef>      // offsetof()
#include <iomanip>      // std::hex, std::setfill(), std::setw()
#include <sstream>      // std::ostringstream

Network::Address::Address(const sockaddr* addr, socklen_t addrlen) :
    value(reinterpret_cast<const char*>(addr), addrlen)
{
}

bool Network::Address::operator<(const Address& other) const
{
    return (family() < other.family() ||
            port() < other.port() ||
            text() < other.text());
}

bool Network::Address::operator>(const Address& other) const
{
    return (family() > other.family() ||
            port() > other.port() ||
            text() > other.text());
}

bool Network::Address::operator==(const Address& other) const
{
    return (family() == other.family() &&
            port() == other.port() &&
            text() == other.text());
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

std::string Network::Address::path() const
{
    switch(sa_family()) {
#ifndef _WIN32
    case AF_UNIX:
        return sun_text();
#endif
    default:
        return "";
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
    family_type family = sa.sa_family;
    return family;
}

socklen_t Network::Address::sa_length() const
{
#ifdef HAVE_SOCKADDR_SA_LEN
    const sockaddr& sa = static_cast<const sockaddr&>(*this);
    socklen_t length = sa.sa_len;
#else
    socklen_t length = addrlen();
#endif
    assert(length == addrlen());
    return length;
}

std::string Network::Address::sa_text() const
{
    const char* data = value.data();
    std::size_t size = value.size();
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
    in_addr addr = sin.sin_addr;
    return addr;
}

Network::Address::family_type Network::Address::sin_family() const
{
    const sockaddr_in& sin = static_cast<const sockaddr_in&>(*this);
    family_type family = sin.sin_family;
    return family;
}

Network::Address::port_type Network::Address::sin_port() const
{
    const sockaddr_in& sin = static_cast<const sockaddr_in&>(*this);
    port_type port = sin.sin_port;
    return port;
}

std::string Network::Address::sin_text() const
{
    Buffer buffer(INET_ADDRSTRLEN);
    const sockaddr_in& sin = static_cast<const sockaddr_in&>(*this);
    inet_ntop(AF_INET, &sin.sin_addr, &buffer[0], buffer.size());
    return buffer;
}

Network::Address::operator const sockaddr_in6&() const
{
    return *reinterpret_cast<const sockaddr_in6*>(value.data());
}

in6_addr Network::Address::sin6_addr() const
{
    const sockaddr_in6& sin6 = static_cast<const sockaddr_in6&>(*this);
    in6_addr addr6 = sin6.sin6_addr;
    return addr6;
}

Network::Address::family_type Network::Address::sin6_family() const
{
    const sockaddr_in6& sin6 = static_cast<const sockaddr_in6&>(*this);
    family_type family = sin6.sin6_family;
    return family;
}

Network::Address::port_type Network::Address::sin6_port() const
{
    const sockaddr_in6& sin6 = static_cast<const sockaddr_in6&>(*this);
    port_type port = sin6.sin6_port;
    return port;
}

std::string Network::Address::sin6_text() const
{
    Buffer buffer(INET6_ADDRSTRLEN);
    const in6_addr& addr = sin6_addr();
    inet_ntop(AF_INET6, &addr, &buffer[0], buffer.size());
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
    port_type port = sun.sun_family;
    return port;
}

std::string Network::Address::sun_text() const
{
    const sockaddr_un& sun = static_cast<const sockaddr_un&>(*this);
    std::string path(sun.sun_path);
    return path;
}
#endif
