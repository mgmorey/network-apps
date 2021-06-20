#include "network-string.h"

#include <iomanip>      // std::hex
#include <sstream>      // std::ostringstream

static std::string to_string(const char* data, std::size_t size)
{
    std::ostringstream os;
    os << std::hex
       << "0x";

    for(const char* p = data; p < data + size; p++) {
        short ch = static_cast<short>(*p & 0xFF);
        os << std::setfill('0')
           << std::setw(2)
           << ch;
    }

    return os.str();
}

std::string to_string(const struct sockaddr* sa, socklen_t len)
{
    return to_string(reinterpret_cast<const char*>(sa),
                     len);
}

std::string to_string(const struct addrinfo& ai)
{
    return to_string(reinterpret_cast<const char*>(ai.ai_addr),
                     ai.ai_addrlen);
}

std::string to_string(const std::string& addr)
{
    return to_string(addr.data(), addr.size());
}
