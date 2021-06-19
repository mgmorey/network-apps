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
        std::string data() const;
        std::size_t size() const;

    private:
        static std::string& trim(std::string& str);
        std::string str;
    };
}

#endif
