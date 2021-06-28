#include "network-sockaddr.h"   // SockAddr, operator<<(), struct
                                // sockaddr, socklen_t, std::ostream,
                                // std::string
#include "network-buffer.h"     // Buffer
#include "network-family.h"     // Family, operator<<()
#include "network-format.h"     // Format, operator<<()

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, PF_INET, PF_INET6,
                        // inet_ntop()
#else
#include <arpa/inet.h>  // inet_ntop()
#include <sys/socket.h> // AF_INET, AF_INET6, PF_INET, PF_INET6
#endif

#include <cassert>      // assert()
#include <cstddef>      // offsetof()
#include <iomanip>      // std::hex, std::setfill(), std::setw()
#include <sstream>      // std::istringstream, std::ostringstream

Network::SockAddr::SockAddr(const sockaddr* addr, socklen_t len) :
    value(reinterpret_cast<const char*>(addr), len)
{
}

bool Network::SockAddr::operator<(const SockAddr& other) const
{
    return (family() < other.family() ||
            port() < other.port() ||
            addr() < other.addr());
}

bool Network::SockAddr::operator>(const SockAddr& other) const
{
    return (family() > other.family() ||
            port() > other.port() ||
            addr() > other.addr());
}

bool Network::SockAddr::operator==(const SockAddr& other) const
{
    return (family() == other.family() &&
            port() == other.port() &&
            addr() == other.addr());
}

Network::SockAddr::operator const sockaddr*() const
{
    return reinterpret_cast<const sockaddr*>(value.data());
}

Network::SockAddr::operator const sockaddr&() const
{
    return *static_cast<const sockaddr*>(*this);
}

Network::SockAddr::operator const sockaddr_in*() const
{
    return reinterpret_cast<const sockaddr_in*>(value.data());
}

Network::SockAddr::operator const sockaddr_in&() const
{
    return *static_cast<const sockaddr_in*>(*this);
}

Network::SockAddr::operator const sockaddr_in6*() const
{
    return reinterpret_cast<const sockaddr_in6*>(value.data());
}

Network::SockAddr::operator const sockaddr_in6&() const
{
    return *static_cast<const sockaddr_in6*>(*this);
}

Network::SockAddr::operator std::string() const
{
    return value;
}

std::string Network::SockAddr::addr() const
{
    switch(sa_family()) {
    case AF_INET:
        return to_string(sin_addr());
    case AF_INET6:
        return to_string(sin6_addr());
    }

    return to_hexadecimal(sa_data());
}

unsigned Network::SockAddr::family() const
{
    switch(sa_family()) {
    case AF_INET:
        return sin_family();
    case AF_INET6:
        return sin6_family();
    }

    return sa_family();
}

unsigned Network::SockAddr::port() const
{
    switch(sa_family()) {
    case AF_INET:
        return htons(sin_port());
    case AF_INET6:
        return htons(sin6_port());
    }

    return 0;
}

socklen_t Network::SockAddr::size() const
{
    return static_cast<socklen_t>(value.size());
}

std::string Network::SockAddr::sa_data() const
{
    std::size_t offset = offsetof(sockaddr, sa_data);
    return std::string(value.data() + offset,
                       value.size() - offset);
}

unsigned Network::SockAddr::sa_family() const
{
    const sockaddr& sa = static_cast<const sockaddr&>(*this);
    unsigned family = sa.sa_family;
    return family;
}

unsigned Network::SockAddr::sa_length() const
{
#ifdef SOCKADDR_HAS_SA_LEN
    const sockaddr& sa = static_cast<const sockaddr&>(*this);
    unsigned length = sa.sa_len;
#else
    unsigned length = value.size();
#endif
    assert(length == value.size());
    return length;
}

in_addr Network::SockAddr::sin_addr() const
{
    const sockaddr_in& sin = static_cast<const sockaddr_in&>(*this);
    in_addr addr = sin.sin_addr;
    return addr;
}

unsigned Network::SockAddr::sin_family() const
{
    const sockaddr_in& sin = static_cast<const sockaddr_in&>(*this);
    unsigned family = sin.sin_family;
    return family;
}

unsigned Network::SockAddr::sin_port() const
{
    const sockaddr_in& sin = static_cast<const sockaddr_in&>(*this);
    unsigned port = sin.sin_port;
    return port;
}

in6_addr Network::SockAddr::sin6_addr() const
{
    const sockaddr_in6& sin = static_cast<const sockaddr_in6&>(*this);
    in6_addr addr = sin.sin6_addr;
    return addr;
}

unsigned Network::SockAddr::sin6_family() const
{
    const sockaddr_in6& sin = static_cast<const sockaddr_in6&>(*this);
    unsigned family = sin.sin6_family;
    return family;
}

unsigned Network::SockAddr::sin6_port() const
{
    const sockaddr_in6& sin = static_cast<const sockaddr_in6&>(*this);
    unsigned port = sin.sin6_port;
    return port;
}

std::string Network::to_hexadecimal(const std::string& value)
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

    std::string result("0x");
    std::string str(oss.str());
    assert(str.size() == size * 2);

    if (str.empty()) {
        result += "0";
    }
    else {
        result += str;
    }

    return result;
}

std::string Network::to_string(const in_addr& addr)
{
    Buffer buffer(INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &addr, &buffer[0], buffer.size());
    return buffer;
}

std::string Network::to_string(const in6_addr& addr)
{
    Buffer buffer(INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, &addr, &buffer[0], buffer.size());
    return buffer;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const SockAddr& sa)
{
    static const std::string delim(", ");
    static const int tabs[1] = {0};

    unsigned family = sa.sa_family();
    unsigned length = sa.sa_length();

    if (sa.value.size()) {
        os << "sockaddr("
           << Format("sa_len")
           << length;

        switch (family) {
        case AF_INET:
            os << Format(delim, tabs[0], "sin_family")
               << Family(sa.sin_family())
               << Format(delim, tabs[0], "sin_port")
               << htons(sa.sin_port())
               << Format(delim, tabs[0], "sin_addr")
               << '"'
               << to_string(sa.sin_addr())
               << '"';
            break;
        case AF_INET6:
            os << Format(delim, tabs[0], "sin6_family")
               << Family(sa.sin6_family())
               << Format(delim, tabs[0], "sin6_port")
               << htons(sa.sin6_port())
               << Format(delim, tabs[0], "sin6_addr")
               << '"'
               << to_string(sa.sin6_addr())
               << '"';
            break;
        default:
            os << Format(delim, tabs[0], "sa_family")
               << Family(family)
               << Format(delim, tabs[0], "sa_data")
               << to_hexadecimal(sa.sa_data());
        }

        os << ')';
    }
    else {
        os << "0x0";
    }

    return os;
}
