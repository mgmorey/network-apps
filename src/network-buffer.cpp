#include "network-buffer.h"     // Buffer

Network::Buffer::Buffer(std::size_t size) :
    value(size, '\0')
{
}

char& Network::Buffer::operator[](std::size_t pos)
{
    return value[pos];
}

const char& Network::Buffer::operator[](std::size_t pos) const
{
    return value[pos];
}

Network::Buffer::operator std::string() const
{
    std::string result(value);
    std::size_t pos = result.find('\0');

    if (pos != std::string::npos) {
        result.erase(pos);
    }

    return result;
}

std::size_t Network::Buffer::size() const
{
    return value.size();
}
