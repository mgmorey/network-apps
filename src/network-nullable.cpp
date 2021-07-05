#include "network-nullable.h"   // Nullable

Network::Nullable::Nullable(const std::string &other) :
    value_is_null(false),
    value(other)
{
}

Network::Nullable::Nullable(const char* other) :
    value_is_null(other == NULL),
    value(other == NULL ? "" : other)
{
}

Network::Nullable& Network::Nullable::operator=(const std::string &other)
{
    value_is_null = false;
    value = other;
    return *this;
}

Network::Nullable& Network::Nullable::operator=(const char* other)
{
    value_is_null = other == NULL;
    value = other == NULL ? "" : other;
    return *this;
}

Network::Nullable::operator std::string() const
{
    return value;
}

bool Network::Nullable::null() const
{
    return value_is_null;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Nullable& nullable)
{
    if (nullable.value_is_null) {
        os << "0x0";
    }
    else {
        os << '"' << nullable.value << '"';
    }

    return os;
}
