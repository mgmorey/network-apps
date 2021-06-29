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

Network::SockAddr::SockAddr(const sockaddr* addr, socklen_t addrlen) :
    value(reinterpret_cast<const char*>(addr), addrlen)
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
    return *reinterpret_cast<const sockaddr*>(value.data());
}

Network::SockAddr::operator const sockaddr_in&() const
{
    return *reinterpret_cast<const sockaddr_in*>(value.data());
}

Network::SockAddr::operator const sockaddr_in6&() const
{
    return *reinterpret_cast<const sockaddr_in6*>(value.data());
}

std::string Network::SockAddr::addr() const
{
    switch(sa_family()) {
    case AF_INET:
        return to_string(sin_addr());
    case AF_INET6:
        return to_string(sin6_addr());
    }

    return to_string(sa_data());
}

Network::family_type Network::SockAddr::family() const
{
    switch(sa_family()) {
    case AF_INET:
        return sin_family();
    case AF_INET6:
        return sin6_family();
    }

    return sa_family();
}

Network::port_type Network::SockAddr::port() const
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

std::string Network::SockAddr::to_string(const std::string& value)
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

std::string Network::SockAddr::to_string(const in_addr& addr)
{
    Buffer buffer(INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &addr, &buffer[0], buffer.size());
    return buffer;
}

std::string Network::SockAddr::to_string(const in6_addr& addr)
{
    Buffer buffer(INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, &addr, &buffer[0], buffer.size());
    return buffer;
}

std::string Network::SockAddr::sa_data() const
{
    std::size_t offset = offsetof(sockaddr, sa_data);
    return std::string(value.data() + offset,
                       value.size() - offset);
}

Network::family_type Network::SockAddr::sa_family() const
{
    const sockaddr& sa = static_cast<const sockaddr&>(*this);
    family_type family = sa.sa_family;
    return family;
}

socklen_t Network::SockAddr::sa_length() const
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

in_addr Network::SockAddr::sin_addr() const
{
    const sockaddr_in& sin = static_cast<const sockaddr_in&>(*this);
    in_addr addr = sin.sin_addr;
    return addr;
}

Network::family_type Network::SockAddr::sin_family() const
{
    const sockaddr_in& sin = static_cast<const sockaddr_in&>(*this);
    family_type family = sin.sin_family;
    return family;
}

Network::port_type Network::SockAddr::sin_port() const
{
    const sockaddr_in& sin = static_cast<const sockaddr_in&>(*this);
    port_type port = sin.sin_port;
    return port;
}

in6_addr Network::SockAddr::sin6_addr() const
{
    const sockaddr_in6& sin = static_cast<const sockaddr_in6&>(*this);
    in6_addr addr = sin.sin6_addr;
    return addr;
}

Network::family_type Network::SockAddr::sin6_family() const
{
    const sockaddr_in6& sin = static_cast<const sockaddr_in6&>(*this);
    family_type family = sin.sin6_family;
    return family;
}

Network::port_type Network::SockAddr::sin6_port() const
{
    const sockaddr_in6& sin = static_cast<const sockaddr_in6&>(*this);
    port_type port = sin.sin6_port;
    return port;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const SockAddr& sa)
{
    static const std::string delim(", ");
    static const int tabs[1] = {0};

    family_type family = sa.sa_family();
    socklen_t length = sa.sa_length();

    if (sa.value.size()) {
        switch (family) {
        case AF_INET:
            os << static_cast<const sockaddr_in&>(sa);
            break;
        case AF_INET6:
            os << static_cast<const sockaddr_in6&>(sa);
            break;
        default:
            os << "sockaddr("
               << Format("sa_len")
               << length
               << Format(delim, tabs[0], "sa_family")
               << Family(family)
               << Format(delim, tabs[0], "sa_data")
               << SockAddr::to_string(sa.sa_data())
               << ')';
        }
    }
    else {
        os << "0x0";
    }

    return os;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const sockaddr_in& sin)
{
    static const std::string delim(", ");
    static const int tabs[1] = {0};

    os << "sockaddr_in("
       << Format("sin_family")
       << Family(sin.sin_family)
       << Format(delim, tabs[0], "sin_port")
       << htons(sin.sin_port)
       << Format(delim, tabs[0], "sin_addr")
       << sin.sin_addr
       << ')';
    return os;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const sockaddr_in6& sin)
{
    static const std::string delim(", ");
    static const int tabs[1] = {0};

    os << "sockaddr_in6("
       << Format("sin6_family")
       << Family(sin.sin6_family)
       << Format(delim, tabs[0], "sin6_port")
       << htons(sin.sin6_port)
       << Format(delim, tabs[0], "sin6_addr")
       << sin.sin6_addr
       << ')';
    return os;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const in_addr& addr)
{
    os << "in_addr("
       << SockAddr::to_string(addr)
       << ')';
    return os;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const in6_addr& addr)
{
    os << "in6_addr("
       << SockAddr::to_string(addr)
       << ')';
    return os;
}
