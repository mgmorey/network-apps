#include "network-string.h"

#include <iomanip>      // std::hex
#include <sstream>      // std::ostringstream

std::string to_hex(const struct sockaddr* data, socklen_t size)
{
    return to_hex(reinterpret_cast<const char*>(data), size);
}

std::string to_hex(const char* data, std::size_t size)
{
    std::ostringstream os;
    os << std::hex;

    for(std::size_t i = 0; i < size; i++) {
        os << static_cast<unsigned>(data[i]);
    }

    return os.str();
}

std::string to_hex(const std::string& str)
{
    return to_hex(str.data(), str.size());
}
