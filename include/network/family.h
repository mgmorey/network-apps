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
        friend std::ostream& operator<<(std::ostream& os,
                                        const Family& family);

    public:
        explicit Family(family_type t_value);
        Family& operator=(family_type t_value);
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Family& family);
}

#endif
