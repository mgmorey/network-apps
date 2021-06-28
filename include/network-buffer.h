#ifndef NETWORK_BUFFER_H
#define NETWORK_BUFFER_H

#include <cstddef>      // std::size_t
#include <string>       // std::string

namespace Network
{
    class Buffer
    {
    public:
        Buffer(std::size_t size = 0);
        char& operator[](std::size_t pos);
        const char& operator[](std::size_t pos) const;
        operator std::string() const;
        std::size_t size() const;

    private:
        std::string value;
    };
}

#endif
