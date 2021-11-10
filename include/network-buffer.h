#ifndef NETWORK_BUFFER_H
#define NETWORK_BUFFER_H

#include <string>       // std::string

namespace Network
{
    class Buffer
    {
    public:
        explicit Buffer(std::string::size_type t_size = 0);
        operator std::string() const;
        const char* data() const;
        char* data();
        std::string::size_type size() const;

    private:
        std::string m_value;
    };
}

#endif
