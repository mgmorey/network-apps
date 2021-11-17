#ifndef NETWORK_FAMILY_H
#define NETWORK_FAMILY_H

#include "network/integer.h"        // Integer

#include <ostream>      // std::ostream

namespace Network
{
    class Family :
        public Integer<unsigned short>
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Family& family);

    public:
        explicit Family(unsigned short t_value);
        Family& operator=(unsigned short t_value);
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Family& family);
}

#endif
