#include "network-string.h"

#include <cassert>      // assert()
#include <iomanip>      // std::hex, std::setfill(), std::setw()
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

    assert(os.str().length() == size * 2 + 2);
    return os.str();
}

std::string to_string(const struct sockaddr* data, socklen_t size)
{
    return to_string(reinterpret_cast<const char*>(data),
                     size);
}
