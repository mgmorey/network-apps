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
#include <sstream>      // std::istringstream, std::ostringstream

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

std::string Network::SockAddr::sa_data() const
{
    std::size_t offset = offsetof(sockaddr, sa_data);
    return std::string(value.data() + offset,
                       value.size() - offset);
}

int Network::SockAddr::sa_family() const
{
    const sockaddr* psa = static_cast<const sockaddr*>(*this);
    int family = psa->sa_family;
    return family;
}

int Network::SockAddr::sa_length() const
{
#ifdef SOCKADDR_HAS_SA_LEN
    const sockaddr* psa = static_cast<const sockaddr*>(*this);
    int length = psa->sa_len;
#else
    int length = value.size();
#endif
    assert(length = value.size());
    return length;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const SockAddr& sa)
{
    static const std::string delim(", ");
    static const int tabs[1] = {0};

    if (sa.value.size()) {
        os << "sockaddr("
           << Format("sa_len")
           << sa.sa_length()
           << Format(delim, tabs[0], "sa_family")
           << Family(sa.sa_family())
           << Format(delim, tabs[0], "sa_data")
           << to_hexadecimal(sa.sa_data())
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
