#include "network/result.h"     // Result

#include <ostream>      // std::ostream
#include <string>       // std::string
#include <utility>      // std::move()

Network::Result::Result(result_type t_result, std::string t_string) :
    m_result(t_result),
    m_string(std::move(t_string))
{
}

Network::Result::operator bool() const
{
    return m_result != 0 || !m_string.empty();
}

auto Network::Result::result() const -> Network::result_type
{
    return m_result;
}

auto Network::Result::string() const -> std::string
{
    return m_string;
}

auto Network::operator<<(std::ostream& os,
                         const Result& result) -> std::ostream&
{
    return os << result.string();
}
