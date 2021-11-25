#include "network/result.h"     // Result

#include <ostream>      // std::ostream
#include <string>       // std::string
#include <utility>      // std::utility

Network::Result::Result(result_type t_result, const std::string& t_string) :
    m_result(t_result),
    m_string(t_string)
{
}

Network::Result::Result(result_type t_result, std::string&& t_string) :
    m_result(t_result),
    m_string(std::move(t_string))
{
}

Network::Result::operator bool() const
{
    return m_result != 0 || !m_string.empty();
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
