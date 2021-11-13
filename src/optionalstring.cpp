#include "network/optionalstring.h" // OptionalString

Network::OptionalString::OptionalString(const std::string &t_string) :
    Optional(false),
    m_value(t_string)
{
}

Network::OptionalString::OptionalString(const char* t_value) :
    Optional(t_value == nullptr),
    m_value(t_value == nullptr ? "" : t_value)
{
}

Network::OptionalString&
Network::OptionalString::operator=(const std::string &t_string)
{
    m_null = false;
    m_value = t_string;
    return *this;
}

Network::OptionalString&
Network::OptionalString::operator=(const char* t_value)
{
    m_null = (t_value == nullptr);
    m_value = (t_value == nullptr) ? "" : t_value;
    return *this;
}

Network::OptionalString::operator std::string() const
{
    return m_value;
}

Network::OptionalString::operator const char*() const
{
    return m_null ? nullptr : m_value.c_str();
}

const char* Network::OptionalString::data() const
{
    return m_value.data();
}

char* Network::OptionalString::data()
{
    return m_value.data();
}

bool Network::OptionalString::empty() const
{
    return m_value.empty();
}

std::string::size_type Network::OptionalString::length() const
{
    return m_value.length();
}

std::string::size_type Network::OptionalString::size() const
{
    return m_value.size();
}
