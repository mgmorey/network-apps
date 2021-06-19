#ifndef NETWORK_RESULT_H
#define NETWORK_RESULT_H

#include <string>       // std::string

namespace Network
{
    class Result
    {
    public:
        Result();
        Result(int value, std::string str);
        bool nonzero() const;
        int result() const;
        std::string string() const;

    private:
        int result_value;
        std::string result_string;
    };
}

#endif
