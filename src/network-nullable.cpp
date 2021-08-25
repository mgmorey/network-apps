#include "network-nullable.h"   // Nullable

Network::Nullable::Nullable() :
    m_value_is_null(false)
{
}

Network::Nullable::Nullable(const std::string &t_nullable) :
    m_value_is_null(false),
    m_value(t_nullable)
{
}

Network::Nullable::Nullable(const char* t_value) :
    m_value_is_null(t_value == NULL),
    m_value(t_value == NULL ? "" : t_value)
{
}

Network::Nullable& Network::Nullable::operator=(const std::string &t_nullable)
{
    m_value_is_null = false;
    m_value = t_nullable;
    return *this;
}

Network::Nullable& Network::Nullable::operator=(const char* t_value)
{
    m_value_is_null = t_value == NULL;
    m_value = t_value == NULL ? "" : t_value;
    return *this;
}

Network::Nullable::operator std::string() const
{
    return m_value;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Nullable& nullable)
{
    if (nullable.m_value_is_null) {
        os << "0x0";
    }
    else {
        os << '"' << nullable.m_value << '"';
    }

    return os;
}
