#ifndef NETWORK_RESULT_H
#define NETWORK_RESULT_H

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    struct Result
    {
        Result();
        Result(int t_value, const std::string& t_string);
        int result() const;
        std::string string() const;

    private:
        std::string m_string;
        int m_value {0};
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Result& result);
}

#endif
