#ifndef NETWORK_SOCKTYPE_H
#define NETWORK_SOCKTYPE_H

#include "network/integer.h"        // Integer
#include "network/socketargument.h" // socket_argument_type

#include <ostream>      // std::ostream

namespace Network
{
    using socktype_type = socket_argument_type;

    class SockType :
        public Integer<socktype_type>
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const SockType& socktype);

    public:
        explicit SockType(socktype_type t_value);
        SockType& operator=(socktype_type t_value);
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const SockType& socktype);
}

#endif
