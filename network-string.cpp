#include "network-string.h"     // resize()

std::string Network::resize(const std::string& str)
{
    std::string result(str);
    std::string::size_type pos = result.find('\0');

    if (pos != std::string::npos) {
        result.resize(pos);
    }

    return result;
}
