#ifndef NETWORK_FLAGS_H
#define NETWORK_FLAGS_H

#include "network-integer.h"    // Integer

#include <ostream>      // std::ostream

namespace Network
{
    class Flags :
        public Integer<int>
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Flags& flags);

    public:
        explicit Flags(int t_value);
        Flags& operator=(int t_value);
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Flags& flags);
}

#endif
