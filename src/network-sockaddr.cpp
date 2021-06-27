#include "network-sockaddr.h"   // SockAddr, operator<<(), struct
                                // sockaddr, socklen_t, std::ostream,
                                // std::string
#include "network-family.h"     // Family, operator<<()

#include <cassert>      // assert()
#include <iomanip>	// std::hex
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
    unsigned short family = 0;

    switch (size) {
    case 16:
    case 28:
        family = *(reinterpret_cast<const unsigned short*>(data));

        switch (family) {
        case AF_INET:
        case AF_INET6:
            data += sizeof family;
            size -= sizeof family;
            break;
        }
    }

    return std::string(data, size);
}

unsigned short Network::SockAddr::family() const
{
    const char* data = value.data();
    std::size_t size = value.size();
    unsigned short family = 0;

    switch (size) {
    case 16:
    case 28:
        family = *(reinterpret_cast<const unsigned short*>(data));
    }

    return family;
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

socklen_t Network::SockAddr::length() const
{
    return static_cast<socklen_t>(value.size());
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const SockAddr& sa)
{
    return os << "sockaddr(sa_family="
              << Family(sa.family())
              << ", sa_data="
              << SockAddr::to_hexadecimal(sa.data())
              << ')';
}
