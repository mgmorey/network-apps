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

Network::Integer::operator int() const
{
    return m_value;
}
