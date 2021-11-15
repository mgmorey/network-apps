#ifndef NETWORK_BUFFER_H
#define NETWORK_BUFFER_H

#include <string>       // std::string

namespace Network
{
    class Buffer
    {
    public:
        explicit Buffer(std::string::size_type t_size = 0) :
            m_value(t_size, '\0')
        {
        }

        operator std::string() const
        {
            return m_value.substr(0, m_value.find('\0'));
        }

        char* data()
        {
            return m_value.data();
        }

        std::string::size_type size() const
        {
            return m_value.size();
        }

    private:
        std::string m_value;
    };
}

#endif
