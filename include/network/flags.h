#ifndef NETWORK_FLAGS_H
#define NETWORK_FLAGS_H

#include "network/integer.h"        // Integer

#include <ostream>      // std::ostream

namespace Network
{
    class Flags :
        public Integer<unsigned short>
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Flags& flags);

    public:
        explicit Flags(unsigned short t_value);
        Flags& operator=(unsigned short t_value);
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Flags& flags);
}

#endif
