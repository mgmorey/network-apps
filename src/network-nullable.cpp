#include "network-nullable.h"   // Nullable

#include <ostream>      // std::ostream

Network::Nullable::Nullable() :
    m_is_null(true)
{
}

Network::Nullable::Nullable(const std::string &t_nullable) :
    m_is_null(false),
    m_value(t_nullable)
{
}

Network::Nullable::Nullable(const char* t_value) :
    m_is_null(t_value == nullptr),
    m_value(t_value == nullptr ? "" : t_value)
{
}

Network::Nullable& Network::Nullable::operator=(const std::string &t_nullable)
{
    m_is_null = false;
    m_value = t_nullable;
    return *this;
}

Network::Nullable& Network::Nullable::operator=(const char* t_value)
{
    m_is_null = t_value == nullptr;
    m_value = t_value == nullptr ? "" : t_value;
    return *this;
}

Network::Nullable::operator std::string() const
{
    return m_value;
}

const char* Network::Nullable::c_str() const
{
    return m_is_null ? nullptr : m_value.c_str();
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Nullable& nullable)
{
    if (nullable.m_is_null) {
        os << "0x0";
    }
    else {
        os << '"' << nullable.m_value << '"';
    }

    return os;
}
