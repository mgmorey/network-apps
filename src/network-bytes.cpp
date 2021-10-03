#include "network-bytes.h"      // Bytes

#include <string>       // std::string

Network::Bytes::Bytes()
{
}

Network::Bytes::Bytes(const std::byte* begin, const std::byte* end) :
    m_bytes(begin, end)
{
}

std::byte& Network::Bytes::operator[](std::size_t t_index)
{
    return m_bytes[t_index];
}

const std::byte& Network::Bytes::operator[](std::size_t t_index) const
{
    return m_bytes[t_index];
}

std::size_t Network::Bytes::size() const
{
    return m_bytes.size();
}

std::string Network::to_string(const Bytes& t_bytes)
{
    std::string result(t_bytes.size(), '\0');

    for (std::string::size_type i = 0; i < t_bytes.size(); ++i) {
        result[i] = static_cast<std::string::value_type>(t_bytes[i]);
    }

    return result;
}
