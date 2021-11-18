#ifndef NETWORK_FLAGS_H
#define NETWORK_FLAGS_H

#include "network/integer.h"        // Integer
#include "network/socketargument.h" // socket_argument_type

#include <ostream>      // std::ostream

namespace Network
{
    using flags_type = socket_argument_type;

    class Flags :
        public Integer<flags_type>
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Flags& flags);

    public:
        explicit Flags(flags_type t_value);
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Flags& flags);
}

#endif
