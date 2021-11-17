#ifndef NETWORK_SOCKTYPE_H
#define NETWORK_SOCKTYPE_H

#include "network/integer.h"        // Integer

#include <ostream>      // std::ostream

namespace Network
{
    class SockType :
        public Integer<unsigned short>
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const SockType& socktype);

    public:
        explicit SockType(unsigned short t_value);
        SockType& operator=(unsigned short t_value);
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const SockType& socktype);
}

#endif
