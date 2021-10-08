#include "network-string.h"   // String

Network::String::String() :
    Optional(true)
{
}

Network::String::String(const std::string &t_string) :
    Optional(false),
    m_value(t_string)
{
}

Network::String::String(const char* t_value) :
    Optional(t_value == nullptr),
    m_value(t_value == nullptr ? "" : t_value)
{
}

Network::String& Network::String::operator=(const std::string &t_string)
{
    m_null = false;
    m_value = t_string;
    return *this;
}

Network::String& Network::String::operator=(const char* t_value)
{
    m_null = (t_value == nullptr);
    m_value = (t_value == nullptr) ? "" : t_value;
    return *this;
}

Network::String::operator std::string() const
{
    return m_value;
}

Network::String::operator const char*() const
{
    return m_null ? nullptr : m_value.c_str();
}

bool Network::String::empty() const
{
    return m_value.empty();
}
