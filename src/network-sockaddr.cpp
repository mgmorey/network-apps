#include "network-sockaddr.h"   // SockAddr, operator<<(), struct
                                // sockaddr, socklen_t, std::ostream,
                                // std::string
#include "network-family.h"     // Family, operator<<()
#include "network-format.h"     // Format, operator<<()

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, PF_INET, PF_INET6
#else
#include <sys/socket.h> // AF_INET, AF_INET6, PF_INET, PF_INET6
#endif

#include <cassert>      // assert()
#include <iomanip>      // std::hex
#include <sstream>      // std::ostringstream

Network::SockAddr::SockAddr(const sockaddr* addr, socklen_t len) :
    value(reinterpret_cast<const char*>(addr), len)
{
}

bool Network::SockAddr::operator<(const SockAddr& other) const
{
    return value < other.value;
}

bool Network::SockAddr::operator>(const SockAddr& other) const
{
    return value > other.value;
}

bool Network::SockAddr::operator==(const SockAddr& other) const
{
    return value == other.value;
}

std::string Network::SockAddr::data() const
{
    const char* data = value.data();
    std::size_t size = value.size();
#ifdef SOCKADDR_HAS_SA_LEN
    extract_length(data, size);
#endif
    extract_family(data, size);
    return std::string(data, size);
}

unsigned short Network::SockAddr::family() const
{
    const char* data = value.data();
    std::size_t size = value.size();
#ifdef SOCKADDR_HAS_SA_LEN
    extract_length(data, size);
#endif
    return extract_family(data, size);
}

unsigned short Network::SockAddr::length() const
{
#ifdef SOCKADDR_HAS_SA_LEN
    const char* data = value.data();
    std::size_t size = value.size();
    unsigned short length = extract_length(data, size);
    assert(length == value.size());
    return length;
#else
    return value.size();
#endif
}

unsigned Network::SockAddr::extract_family(const char*& data, std::size_t& size)
{
    unsigned short family = *(reinterpret_cast<const unsigned short*>(data));

    switch (family) {
    case AF_INET:
    case AF_INET6:
        data += sizeof family;
        size -= sizeof family;
        break;
    default:
        family = 0;
    }

    return family;
}

unsigned Network::SockAddr::extract_length(const char*& data, std::size_t& size)
{
    unsigned char length = *(reinterpret_cast<const unsigned char*>(data));
    data += sizeof length;
    size -= sizeof length;
    return length;
}

std::string Network::SockAddr::to_hexadecimal(const std::string& value)
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

Network::SockAddr::operator const sockaddr*() const
{
    return reinterpret_cast<const sockaddr*>(value.data());
}

Network::SockAddr::operator std::string() const
{
    return value;
}

socklen_t Network::SockAddr::size() const
{
    return static_cast<socklen_t>(value.size());
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const SockAddr& sa)
{
    if (sa.size()) {
        const std::string delim(", ");
        os << "sockaddr("
           << Format("sa_len")
           << sa.length()
           << Format(delim, 0, "sa_family")
           << Family(sa.family())
           << Format(delim, 0, "sa_data")
           << SockAddr::to_hexadecimal(sa.data())
           << ')';
    }
    else {
        os << "0x0";
    }

    return os;
}
