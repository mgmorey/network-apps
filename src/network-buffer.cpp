#include "network-buffer.h"     // Buffer, std::string

Network::Buffer::Buffer(std::string::size_type t_size) :
    m_value(t_size, '\0')
{
}

Network::Buffer::operator std::string() const
{
    return m_value.substr(0, m_value.find('\0'));
}

const char* Network::Buffer::data() const
{
    return m_value.data();
}

char* Network::Buffer::data()
{
    return m_value.data();
}

std::string::size_type Network::Buffer::size() const
{
    return m_value.size();
}
