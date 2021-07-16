#include "network-buffer.h"     // Buffer, std::string

Network::Buffer::Buffer(std::string::size_type t_size) :
    m_value(t_size, '\0')
{
}

char& Network::Buffer::operator[](std::string::size_type t_pos)
{
    return m_value[t_pos];
}

const char& Network::Buffer::operator[](std::string::size_type t_pos) const
{
    return m_value[t_pos];
}

Network::Buffer::operator std::string() const
{
    std::string result(m_value);
    const std::string::size_type pos = result.find('\0');

    if (pos != std::string::npos) {
        result.erase(pos);
    }

    return result;
}

std::string::size_type Network::Buffer::size() const
{
    return m_value.size();
}
