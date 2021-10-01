#include "network-result.h"     // Result

#include <ostream>      // std::ostream
#include <string>       // std::string

Network::Result::Result()
{
}

Network::Result::Result(result_type t_value, const std::string& t_string) :
    m_string(t_string),
    m_result(t_value)
{
}

Network::result_type Network::Result::result() const
{
    return m_result;
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
