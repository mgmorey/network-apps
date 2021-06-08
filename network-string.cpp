#include "network-string.h"     // resize()

std::string Network::resize(const std::string& str)
{
    std::string::size_type pos = str.find('\0');
    std::string result(str);

    if (pos != std::string::npos) {
        result.resize(pos);
    }

    return result;
}
