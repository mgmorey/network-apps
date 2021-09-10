#ifndef NETWORK_BUFFER_H
#define NETWORK_BUFFER_H

#include <string>       // std::string

namespace Network
{
    class Buffer
    {
    public:
        explicit Buffer(std::string::size_type t_size = 0);
        char& operator[](std::string::size_type t_pos);
        const char& operator[](std::string::size_type t_pos) const;
        explicit operator std::string() const;
        std::string::size_type size() const;

    private:
        std::string m_value;
    };
}

#endif
