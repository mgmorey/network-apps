#ifndef NETWORK_FLAGS_H
#define NETWORK_FLAGS_H

#include "network/flags-type.h"         // flags_type
#include "network/integer.h"            // Integer

#include <ostream>      // std::ostream

namespace Network
{
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
