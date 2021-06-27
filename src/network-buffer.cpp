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
    return trim(result);
}

std::size_t Network::Buffer::size() const
{
    return value.size();
}

std::string& Network::Buffer::trim(std::string& value)
{
    std::size_t pos = value.find('\0');

    if (pos != std::string::npos) {
        value.resize(pos);
    }

    return value;
}
