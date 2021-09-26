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
    const auto pos {m_value.find('\0')};
    return m_value.substr(0, pos);
}

std::string::size_type Network::Buffer::size() const
{
    return m_value.size();
}
