#include "network/buffer.h"         // Buffer

#include <ostream>      // std::ostream
#include <string>       // std::string

Network::Buffer::Buffer(std::string::size_type t_size) :
    m_value(t_size, '\0')
{
}

Network::Buffer::operator std::string() const
{
    return m_value.substr(0, m_value.find('\0'));
}

auto Network::Buffer::data() -> char*
{
    return m_value.data();
}

auto Network::Buffer::size() const -> std::string::size_type
{
    return m_value.size();
}

auto Network::operator<<(std::ostream& os,
                         const Network::Buffer& buffer) -> std::ostream&
{
    return os << std::string(buffer);
}
