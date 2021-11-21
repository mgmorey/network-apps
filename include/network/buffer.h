#ifndef NETWORK_BUFFER_H
#define NETWORK_BUFFER_H

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class Buffer
    {
    public:
        explicit Buffer(std::string::size_type t_size);
        operator std::string() const;  // NOLINT
        char* data();
        [[nodiscard]] std::string::size_type size() const;

    private:
        std::string m_value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Buffer& buffer);
}

#endif
