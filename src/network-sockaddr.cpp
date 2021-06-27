#include "network-sockaddr.h"   // SockAddr, operator<<(), struct
                                // sockaddr, socklen_t, std::ostream,
                                // std::string

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

Network::SockAddr::operator const sockaddr*() const
{
    return reinterpret_cast<const sockaddr*>(value.data());
}

Network::SockAddr::operator std::string() const
{
    return value;
}

const char* Network::SockAddr::data() const
{
    return value.data();
}

socklen_t Network::SockAddr::length() const
{
    return static_cast<socklen_t>(value.size());
}

std::size_t Network::SockAddr::size() const
{
    return value.size();
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const SockAddr& addr)
{
    if (addr.value.empty()) {
        return os << 0;
    }

    std::ostringstream oss;
    oss << std::hex << "0x";
    const char* data = addr.data();
    std::size_t size = addr.size();

    for(const char* p = data; p < data + size; p++) {
        short ch = static_cast<short>(*p & 0xFF);
        oss << std::setfill('0')
            << std::setw(2)
            << ch;
    }

    assert(oss.str().length() == size * 2 + 2);
    return os << oss.str();
}
