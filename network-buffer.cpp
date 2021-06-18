#include "network-buffer.h"     // Buffer

Network::Buffer::Buffer(std::size_t size) :
    str(size, '\0')
{
}

Network::Buffer::~Buffer()
{
}

char& Network::Buffer::operator[](std::size_t pos)
{
    return str[pos];
}

const char& Network::Buffer::operator[](std::size_t pos) const
{
    return str[pos];
}

std::string Network::Buffer::data() const
{
    std::string result(str);
    resize(result);
    return result;
}

std::size_t Network::Buffer::size() const
{
    return str.size();
}

void Network::Buffer::resize(std::string& str)
{
    std::size_t pos = str.find('\0');

    if (pos != std::string::npos) {
        str.resize(pos);
    }
}
