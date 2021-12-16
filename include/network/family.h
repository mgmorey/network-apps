#ifndef NETWORK_FAMILY_H
#define NETWORK_FAMILY_H

#include "network/family-type.h"        // family_type
#include "network/integer.h"            // Integer

#include <ostream>      // std::ostream

namespace Network
{
    class Family :
        public Integer<family_type>
    {
        friend auto operator<<(std::ostream& os,
                               const Family& family) -> std::ostream&;

    public:
        explicit Family(family_type t_value);
    };

    extern auto operator<<(std::ostream& os,
                           const Family& family) -> std::ostream&;
}

#endif
