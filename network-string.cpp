#include "network-string.h"

#include <iomanip>      // std::hex
#include <sstream>      // std::ostringstream

static std::string to_hex(const char* data, std::size_t size)
{
    std::ostringstream os;
    os << std::hex;

    for(const char* p = data; p < data + size; p++) {
        os << static_cast<unsigned>(*p);
    }

    return os.str();
}

std::string to_hex(const struct sockaddr* addr, socklen_t len)
{
    return to_hex(reinterpret_cast<const char*>(addr), len);
}

std::string to_hex(const std::string& str)
{
    return to_hex(str.data(), str.size());
}
