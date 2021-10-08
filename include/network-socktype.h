#ifndef NETWORK_SOCKTYPE_H
#define NETWORK_SOCKTYPE_H

#include "network-integer.h"    // Integer

#include <ostream>      // std::ostream

namespace Network
{
    class SockType :
        public Integer<int>
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const SockType& socktype);

    public:
        explicit SockType(int t_value);
        SockType& operator=(int t_value);
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const SockType& socktype);
}

#endif
