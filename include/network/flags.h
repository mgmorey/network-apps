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
        friend auto operator<<(std::ostream& os,
                               const Flags& flags) -> std::ostream&;

    public:
        explicit Flags(flags_type t_value);
    };

    extern auto operator<<(std::ostream& os,
                           const Flags& flags) -> std::ostream&;
}

#endif
