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
    static_cast<void>(length(data, size));
    static_cast<void>(family(data, size));
    return std::string(data, size);
}

int Network::SockAddr::family() const
{
    const char* data = value.data();
    std::size_t size = value.size();
    static_cast<void>(length(data, size));
    return family(data, size);
}

int Network::SockAddr::length() const
{
    const char* data = value.data();
    std::size_t size = value.size();
    return length(data, size);
}

Network::SockAddr::operator const sockaddr*() const
{
    return reinterpret_cast<const sockaddr*>(value.data());
}

Network::SockAddr::operator std::string() const
{
    return value;
}

int Network::SockAddr::family(const char*& data, std::size_t& size) const
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

int Network::SockAddr::length(const char*& data, std::size_t& size) const
{
#ifdef SOCKADDR_HAS_SA_LEN
    unsigned char length = *(reinterpret_cast<const unsigned char*>(data));
    assert(length == value.size());
    data += sizeof length;
    size -= sizeof length;
    return length;
#else
    static_cast<void>(data);
    static_cast<void>(size);
    return value.size();
#endif
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
           << to_hexadecimal(sa.data())
           << ')';
    }
    else {
        os << "0x0";
    }

    return os;
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
