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
        SockType(int t_value);

    private:
        int m_value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const SockType& socktype);
}

#endif
