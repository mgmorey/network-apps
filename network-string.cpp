#include "network-string.h"

#include <string>       // std::string

void Network::trim_zeros(std::string& str)
{
    std::string::size_type pos = str.find('\0');

    if (pos != std::string::npos) {
        str.resize(pos);
    }
}
