#include "network-result.h"     // Result

Network::Result::Result() :
    m_value(0)
{
}

Network::Result::Result(int t_value, const std::string& t_string) :
    m_string(t_string),
    m_value(t_value)
{
}

bool Network::Result::nonzero() const
{
    return m_value != 0;
}

int Network::Result::result() const
{
    return m_value;
}

std::string Network::Result::string() const
{
    return m_string;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Result& result)
{
    return os << result.string();
}
