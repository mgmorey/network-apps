#ifndef NETWORK_SOCKTYPE_H
#define NETWORK_SOCKTYPE_H

#include <ostream>      // std::ostream

namespace Network
{
    class SockType
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const SockType& socktype);

    public:
        explicit SockType(int t_value);
        SockType& operator=(int t_value);
        explicit operator int() const;

    private:
        int m_value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const SockType& socktype);
}

#endif
