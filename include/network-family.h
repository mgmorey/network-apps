#ifndef NETWORK_FAMILY_H
#define NETWORK_FAMILY_H

#include "network-integer.h"    // Integer

#include <ostream>      // std::ostream

namespace Network
{
    class Family :
        public Integer
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Family& family);

    public:
        explicit Family(int t_value);
        Family& operator=(int t_value);
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Family& family);
}

#endif
