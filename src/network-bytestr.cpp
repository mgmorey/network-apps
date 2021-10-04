#include "network-bytestr.h"    // ByteString

#include <string>       // std::string

Network::ByteString::ByteString()
{
}

Network::ByteString::ByteString(const std::byte* first, const std::byte* last) :
    m_bytes(first, last)
{
}

const std::byte* Network::ByteString::cbegin(std::size_t t_offset) const
{
    return data() + t_offset;
}

const std::byte* Network::ByteString::cend(std::size_t t_offset) const
{
    return data() + size() - t_offset;
}

const std::byte* Network::ByteString::data() const
{
    return m_bytes.data();
}

bool Network::ByteString::empty() const
{
    return m_bytes.empty();
}

std::size_t Network::ByteString::size() const
{
    return m_bytes.size();
}

std::string Network::to_string(const ByteString& bytes)
{
    std::string text(bytes.m_bytes.size(), '\0');

    for (std::string::size_type i = 0; i < bytes.size(); ++i) {
        text[i] = static_cast<char>(bytes.m_bytes[i]);
    }

    return text;
}
