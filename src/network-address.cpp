#include "network-address.h"    // Address, operator<<(), struct
                                // sockaddr, socklen_t, std::ostream,
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

Network::Address::operator const sockaddr*() const
{
    return reinterpret_cast<const sockaddr*>(value.data());
}

Network::Address::operator std::string() const
{
    return text();
}

Network::Address::family_type Network::Address::family() const
{
    switch(sa_family()) {
    case AF_INET:
        return sin_family();
    case AF_INET6:
        return sin6_family();
    }

    return sa_family();
}

Network::Address::port_type Network::Address::port() const
{
    switch(sa_family()) {
    case AF_INET:
        return htons(sin_port());
    case AF_INET6:
        return htons(sin6_port());
    }

    return 0;
}

socklen_t Network::Address::size() const
{
    return static_cast<socklen_t>(value.size());
}

std::string Network::Address::text() const
{
    switch(sa_family()) {
    case AF_INET:
        return to_string(sin_addr());
    case AF_INET6:
        return to_string(sin6_addr());
    }

    return to_string(sa_data());
}

std::string Network::Address::to_string(const std::string& value)
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

std::string Network::Address::to_string(const in_addr& addr)
{
    Buffer buffer(INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &addr, &buffer[0], buffer.size());
    return buffer;
}

std::string Network::Address::to_string(const in6_addr& addr)
{
    Buffer buffer(INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, &addr, &buffer[0], buffer.size());
    return buffer;
}

Network::Address::operator const sockaddr&() const
{
    return *reinterpret_cast<const sockaddr*>(value.data());
}

Network::Address::operator const sockaddr_in&() const
{
    return *reinterpret_cast<const sockaddr_in*>(value.data());
}

Network::Address::operator const sockaddr_in6&() const
{
    return *reinterpret_cast<const sockaddr_in6*>(value.data());
}

#ifndef _WIN32
Network::Address::operator const sockaddr_un&() const
{
    return *reinterpret_cast<const sockaddr_un*>(value.data());
}
#endif

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
    socklen_t length = size();
#endif
    assert(length == size());
    return length;
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

#ifndef _WIN32
Network::Address::family_type Network::Address::sun_family() const
{
    const sockaddr_un& sun = static_cast<const sockaddr_un&>(*this);
    port_type port = sun.sun_family;
    return port;
}

std::string Network::Address::sun_path() const
{
    const sockaddr_un& sun = static_cast<const sockaddr_un&>(*this);
    std::string path(sun.sun_path);
    return path;
}
#endif
