#ifndef NETWORK_RESULT_H
#define NETWORK_RESULT_H

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    using result_type = long long;

    struct Result
    {
        Result();
        Result(result_type t_value, const std::string& t_string);
        result_type result() const;
        std::string string() const;

    private:
        std::string m_string;
        result_type m_result {0};
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Result& result);
}

#endif
