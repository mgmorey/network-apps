#ifndef STREAM_CONTAINER_H
#define STREAM_CONTAINER_H

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    template<typename T>
    std::ostream& print(const T& value,
                        std::ostream& os,
                        const std::string& before = "",
                        const std::string& after = "\n")
    {
        for (auto elem : value) {
            os << before
               << elem
               << after;
        }

        return os;
    }
}

#endif
