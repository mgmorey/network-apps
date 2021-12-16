#ifndef NETWORK_SOCKTYPE_H
#define NETWORK_SOCKTYPE_H

#include "network/integer.h"            // Integer
#include "network/socktype-type.h"      // socktype_type

#include <ostream>      // std::ostream

namespace Network
{
    class SockType :
        public Integer<socktype_type>
    {
        friend auto operator<<(std::ostream& os,
                               const SockType& socktype) -> std::ostream&;

    public:
        explicit SockType(socktype_type t_value);
    };

    extern auto operator<<(std::ostream& os,
                           const SockType& socktype) -> std::ostream&;
}

#endif
