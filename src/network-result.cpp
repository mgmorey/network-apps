#include "network-result.h"     // Result

Network::Result::Result() :
    result_value(0)
{
}

Network::Result::Result(int value, std::string str) :
    result_value(value),
    result_string(str)
{
}

bool Network::Result::nonzero() const
{
    return result_value != 0;
}

int Network::Result::result() const
{
    return result_value;
}

std::string Network::Result::string() const
{
    return result_string;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Result& result)
{
    return os << result.string();
}
