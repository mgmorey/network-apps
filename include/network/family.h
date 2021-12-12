#ifndef NETWORK_FAMILY_H
#define NETWORK_FAMILY_H

#include "network/integer.h"        // Integer
#include "network/socketargument.h" // socket_argument_type

#include <ostream>      // std::ostream

namespace Network
{
    using family_type = socket_argument_type;

    class Family :
        public Integer<family_type>
    {
        friend auto operator<<(std::ostream& os,
                               const Family& family) -> std::ostream&;

    public:
        explicit Family(family_type t_value);
    };

    extern auto operator<<(std::ostream& os,
                           const Family& family) -> std::ostream& ;
}

#endif
