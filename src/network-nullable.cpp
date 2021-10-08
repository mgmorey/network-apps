#include "network-nullable.h"   // Nullable

Network::Nullable::Nullable()
{
}

Network::Nullable::Nullable(const std::string &t_nullable) :
    m_null(false),
    m_value(t_nullable)
{
}

Network::Nullable::Nullable(const char* t_value) :
    m_null(t_value == nullptr),
    m_value(t_value == nullptr ? "" : t_value)
{
}

Network::Nullable& Network::Nullable::operator=(const std::string &t_nullable)
{
    m_null = false;
    m_value = t_nullable;
    return *this;
}

Network::Nullable& Network::Nullable::operator=(const char* t_value)
{
    m_null = (t_value == nullptr);
    m_value = (t_value == nullptr) ? "" : t_value;
    return *this;
}

Network::Nullable::operator std::string() const
{
    return m_value;
}

Network::Nullable::operator const char*() const
{
    return m_null ? nullptr : m_value.c_str();
}

bool Network::Nullable::empty() const
{
    return m_value.empty();
}

bool Network::Nullable::null() const
{
    return m_null;
}
