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
