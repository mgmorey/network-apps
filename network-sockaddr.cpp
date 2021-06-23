#include "network-sockaddr.h"   // SockAddr, operator<<(), struct
                                // sockaddr, socklen_t, std::ostream,
                                // std::string

#include <cassert>      // assert()
#include <iomanip>	// std::hex
#include <sstream>      // std::ostringstream

Network::SockAddr::SockAddr(const struct sockaddr* data, socklen_t size) :
    null(data == NULL),
    value(reinterpret_cast<const char*>(data), size)
{
}

Network::SockAddr::SockAddr(const std::string& other) :
    null(false),
    value(other)
{
}

Network::SockAddr& Network::SockAddr::operator=(const std::string& other)
{
    null = false;
    value = other;
    return *this;
}

Network::SockAddr::operator std::string() const
{
    return value;
}

const struct sockaddr* Network::SockAddr::data() const
{
    return reinterpret_cast<const struct sockaddr*>(value.data());
}

socklen_t Network::SockAddr::size() const
{
    return static_cast<socklen_t>(value.size());
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const SockAddr& sockaddr)
{
    if (sockaddr.null) {
        os << 0;
        return os;
    }

    std::ostringstream oss;
    oss << std::hex
        << "0x";

    const char* data = sockaddr.value.data();
    std::size_t size = sockaddr.value.size();

    for(const char* p = data; p < data + size; p++) {
        short ch = static_cast<short>(*p & 0xFF);
        oss << std::setfill('0')
            << std::setw(2)
            << ch;
    }

    assert(oss.str().length() == size * 2 + 2);
    os << oss.str();
    return os;
}
