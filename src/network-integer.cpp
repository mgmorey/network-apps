#include "network-integer.h"    // Integer

Network::Integer::Integer(int t_value) :
    m_value(t_value)
{
}

Network::Integer& Network::Integer::operator=(int t_value)
{
    m_value = t_value;
    return *this;
}

bool Network::Integer::operator<(const Integer& t_integer) const
{
    return (m_value < static_cast<int>(t_integer.m_value));
}

bool Network::Integer::operator>(const Integer& t_integer) const
{
    return (m_value > static_cast<int>(t_integer.m_value));
}

bool Network::Integer::operator==(const Integer& t_integer) const
{
    return (m_value == static_cast<int>(t_integer.m_value));
}

Network::Integer::operator int() const
{
    return m_value;
}
