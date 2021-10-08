#include "network-optional.h"   // Optional

Network::Optional::Optional()
{
}

Network::Optional::Optional(const std::string &t_optional) :
    m_null(false),
    m_value(t_optional)
{
}

Network::Optional::Optional(const char* t_value) :
    m_null(t_value == nullptr),
    m_value(t_value == nullptr ? "" : t_value)
{
}

Network::Optional& Network::Optional::operator=(const std::string &t_optional)
{
    m_null = false;
    m_value = t_optional;
    return *this;
}

Network::Optional& Network::Optional::operator=(const char* t_value)
{
    m_null = (t_value == nullptr);
    m_value = (t_value == nullptr) ? "" : t_value;
    return *this;
}

Network::Optional::operator std::string() const
{
    return m_value;
}

Network::Optional::operator const char*() const
{
    return m_null ? nullptr : m_value.c_str();
}

bool Network::Optional::empty() const
{
    return m_value.empty();
}

bool Network::Optional::null() const
{
    return m_null;
}
